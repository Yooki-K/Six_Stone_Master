#include "server.h"

Server::Server(QObject *parent) : QTcpServer(parent)
{
    connect(this,SIGNAL(senddktoui(QString)),this->parent()->parent(),SLOT(updatelabel(QString)));
    QTime time= QTime::currentTime();
    qsrand(time.msec()+time.second()*3000);
    while (1) {
        serverdk=qrand();
        if(listen(QHostAddress::Any,serverdk))
            //开启端口服务
        {
            QClipboard *clipboard = QApplication::clipboard();//获取系统剪贴板指针
            clipboard->setText(QString::number(serverdk));//设置剪贴板内容</span>
            QMessageBox::information(NULL,"成功开启服务器","当前服务端口为"+QString::number(serverdk)+",已复制到剪贴板",QMessageBox::Yes);
            emit senddktoui(QString::number(serverdk));
            break;
        }
    }
//    connect(this,SIGNAL(newConnection()),this,SLOT(comingconnect()));//连接客户端
    connect(this,SIGNAL(sendupdateGameInfo(Server*)),this->parent(),SLOT(updateroom(Server*)));//修改游戏大厅
    connect(this,&Server::sendupdateGameInfo,this,[&](){//向每个连接客户端发送最新游戏大厅数据
        QList<MySocket* > allTcpSocket = findChildren<MySocket *>();
        QString data=QString::number(playerFightInfo.size());
        for(int i=0; i<playerFightInfo.size(); i++)
        data +="//"+playerFightInfo[i].first + " PK " + playerFightInfo[i].second;

        for(int i=0; i<allTcpSocket.size(); i++)
        {
        sendMestoc(allTcpSocket.at(i),COMM_SERVER_GAMEINFO, data);
        }
    });
}



//当有服务器连接进来时执行的函数
void Server::incomingConnection(qintptr socketDescriptor)
{
    MySocket*socket=new MySocket(this);//接收
    curconnum ++;
    if(curconnum > maxcurcon)
    {
        sendMestoc(socket,COMM_SERVER_CONN_FAILED,"当前服务器已满");//发送连接失败消息
            curconnum--;
            socket->deleteLater();
            return ;
    }//连接不能超过最大数
    socket->setSocketDescriptor(socketDescriptor);//建立连接
    clientlist.append(socket);//加入到客户端连接列表中
    connect(socket,&MySocket::disconnected,this,[&](){
        curconnum--;
        qintptr descriptor=socket->socketDescriptor();
            for(int i = 0; i < clientlist.count(); i++) {
                if(clientlist.at(i)->socketDescriptor() == descriptor) {
                    clientlist.removeAt(i);
                    emit sendupdatenum(curconnum);
                    break;
                }
            }
        socket->deleteLater();
    });//断开连接删除socket,当前连接数减一
 //connect(socket,SIGNAL(readyRead()),this,SLOT(receiveMessage()));
    connect(socket,&MySocket::readyRead,this,[&](){
        receiveMesfromc(socket);
    });
    QHostAddress clientip(socket->peerAddress());//获得客户端ip
    sendMestoc(socket,COMM_SERVER_CONN_SUCCESSFUL,"IP:"+clientip.toString().mid(7)+"连接成功！");
    emit sendupdatenum(curconnum);
    emit sendupdateGameInfo(this);
}


void Server::sendMestoc(MySocket*socket,comm_request_type ty, QString toc)
{
    toc.prepend(QString::number(int(ty))+"##");
    socket->write(toc.toUtf8());
}

void Server::receiveMesfromc(MySocket *socket)
{
        if(socket->bytesAvailable()>0){//client->socket的有效字节
            QByteArray arr=socket->readAll();
            int i=QString(arr.data()).section("##",0,0).toInt();
            QString s=QString(arr.data()).section("##",1,1);
            QList<MySocket* > allTcpSocket = findChildren<MySocket *>();
            int index=0;
            switch (i) {
            case 6://客户端请求创建主机
            {
                QHostAddress clientip(socket->peerAddress());//获得客户端ip
                playerFightInfo.push_back(qMakePair<QString,QString>(clientip.toString().mid(7),"-"));
                emit sendupdateGameInfo(this);//发送修改游戏大厅信号
                emit openroom(clientip.toString().mid(7));//发送开房信号
            }
                break;
            case 7://客户端请求加入房间
            {
                index=playerFightInfo.indexOf(qMakePair<QString,QString>(s,"-"));
                if(index!=-1){
                    playerFightInfo[index].second=socket->peerAddress().toString().mid(7);
                    emit sendupdateGameInfo(this);
                    for(int j=0;j<allTcpSocket.size();j++){
                        if(allTcpSocket.at(j)->peerAddress().toString().mid(7)==playerFightInfo.at(index).first)
                            allTcpSocket.at(j)->match=socket;
                            socket->match=allTcpSocket.at(j);//建立客户端之间的匹配连接
                    }
                    if(s=="127.0.0.1"){
                        servergame=new Gamemodel(this->parent());//在主窗口中建立线程游戏
                        servergame->type=MM;
                        servergame->AItype=none;
                        servergame->isonline=1;
                        //bool,Gamemodel *game=0, QObject *parent=0 ,  QString name="Player"
                        servergame->player1=new GPlayer(1,servergame,servergame,s);
                        servergame->player2=new GPlayer(0,servergame,servergame,"127.0.0.1");
                        socket->game=servergame;
                        mysocket=socket;
                        serverplayer=servergame->player1;
                        socket->my=servergame->player2;
                        servergame->c=new Chessboard((QWidget*)this->parent(),servergame);
                        servergame->c->show();
                        servergame->start();
                    }//加入服务器房间
                    else
                    {
                        Gamemodel *game=new Gamemodel(this->parent());//在主窗口中建立线程游戏
                        game->isonline=1;
                        game->type=MM;
                        game->AItype=none;
                        //bool,Gamemodel *game=0, QObject *parent=0 ,  QString name="Player"
                        game->player1=new GPlayer(1,game,game,socket->match->peerAddress().toString().mid(7));
                        game->player2=new GPlayer(0,game,game,socket->peerAddress().toString().mid(7));
                        socket->match->game=game;
                        socket->game=game;
                        socket->match->my=game->player1;
                        socket->my=game->player2;
                        game->start();
                    }//加入客户端房间
                }
            }
                break;
            case 8://玩家退出游戏
            {
                emit sendMestoc(socket->match,COMM_CLIENT_QUITGAME,"对方退出游戏");
                socket->match->game=0;
                socket->match->my=0;
                socket->game->deleteLater();
                playerFightInfo.removeOne(QPair<QString,QString>(socket->peerAddress().toString().mid(7),socket->match->peerAddress().toString().mid(7)));
                socket->close();
                emit sendupdateGameInfo(this);
                curconnum--;
            }
                break;
            case 9://玩家游戏操作：落子
            {
                for(int i=0;i<rowline;i++){
                    for(int j=0;j<columnline;j++){
                        socket->game->game_progress[i][j]=((what)QString(s[i*rowline+j]).toInt());
                    }
                }
                if(socket->game->c!=0) socket->game->c->update();
                emit sendMestoc(socket->match,COMM_CLIENT_GAMEOP,QString(arr.data()));//发送新棋盘消息给对家
                emit isokon();
            }
                break;
            case 10://玩家游戏操作：悔棋
                emit sendMestoc(socket->match,COMM_CLIENT_UNDO,"对方请求悔棋，是否同意");
                break;
            case 11://玩家游戏操作：悔棋回复yes
            {
                emit sendMestoc(socket->match,COMM_CLIENT_UNDO_YES,"对方同意你的悔棋请求");
                socket->game->backStep(socket->match->my);
                QString progress="";
                for(int i=0;i<rowline;i++){
                    for(int j=0;j<columnline;j++){
                        progress+=QString::number(int(socket->game->game_progress[i][j]));
                    }
                }
                sendMestoc(socket,COMM_CLIENT_GAMEOP,progress);
                sendMestoc(socket->match,COMM_CLIENT_GAMEOP,progress);
            }
                break;
            case 12://玩家游戏操作：悔棋回复no
                sendMestoc(socket->match,COMM_CLIENT_UNDO_NO,"对方拒绝了你的悔棋请求,请继续游戏");
                break;
            case 13://玩家游戏操作：认输
            {
                sendMestoc(socket->match,COMM_CLIENT_LOSE,"对方认输，你赢了");
                socket->game=0;
                socket->my=0;
                socket->match->game=0;
                socket->match->my=0;
                socket->game->deleteLater();
            }
                break;
            case 16://玩家游戏操作：发送聊天信息
               sendMestoc(socket->match,COMM_CLIENT_CREATE,QString(arr.data()).section("##",1,1));
                break;
            default:
                break;
            }
    }
}

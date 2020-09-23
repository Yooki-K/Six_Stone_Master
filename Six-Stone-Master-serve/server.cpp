#include "server.h"

Server::Server(QObject *parent,QMainWindow*w) : QTcpServer(parent),w(w)
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
    connect(this,SIGNAL(sendupdateGameInfo(Server*)),this->parent(),SLOT(updateroom(Server*)),Qt::DirectConnection);//修改游戏大厅
    connect(this,&Server::sendupdateGameInfo,this,[&](){//向每个连接客户端发送最新游戏大厅数据
        QList<MySocket* > allTcpSocket = findChildren<MySocket *>();
        QString data=QString::number(playerFightInfo.size());
        for(int i=0; i<playerFightInfo.size(); i++)
        data +="//"+playerFightInfo[i].first + " PK " + playerFightInfo[i].second;
        for(int i=0; i<allTcpSocket.size(); i++)
        {
        sendMestoc(allTcpSocket.at(i),COMM_SERVER_GAMEINFO, data);
        }
    },Qt::DirectConnection);
}

void Server::waits(int s)
{
        QEventLoop loop;
        QTimer::singleShot(s,&loop,SLOT(quit()));
        loop.exec();
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
            socket->close();
            return ;
    }//连接不能超过最大数
    socket->setSocketDescriptor(socketDescriptor);//建立连接
    connect(socket,&MySocket::disconnected,this,[&](){
        curconnum--;
       emit sendupdatenum(curconnum);
        int n=clientlist.size();
        for(int i=0; i<n;i++) {
            if(clientlist[i]->write("-1")==-1){
                if(clientlist.at(i)->match!=mysocket)
                    sendMestoc( clientlist.at(i)->match,COMM_CLIENT_QUITGAME,"");
                else
                {
                    if(clientlist.at(i)->game!=0)
                        QMessageBox::information(NULL,"游戏停止","对方退出游戏");
                    else
                        QMessageBox::information(NULL,"提示",clientlist.at(i)->ip+"断开连接");
                    if(clientlist.at(i)->match!=0)
                    {
                        clientlist.at(i)->match->game->stop();
                        delete clientlist.at(i)->match->game;
                        clientlist.at(i)->match->game=0;
                        int index=playerFightInfo.indexOf(QPair<QString,QString>(clientlist.at(i)->ip,clientlist.at(i)->match->ip));
                        if(index==-1)
                            playerFightInfo.removeOne(QPair<QString,QString>(clientlist.at(i)->match->ip,clientlist.at(i)->ip));
                        else
                            playerFightInfo.removeAt(index);
                        if(clientlist.at(i)->match==mysocket)
                        {
                            delete mysocket;
                            mysocket=0;
                        }
                        clientlist.at(i)->clear(1);
                        emit sendupdateGameInfo(this);
                    }

                }
                clientlist.removeAt(i);
            }
        }
    });//断开连接,当前连接数减一
    clientlist.append(socket);
    connect(socket,&MySocket::readyRead,socket,&MySocket::receiveMesfromc);
    connect(socket,SIGNAL(send(MySocket*,QByteArray)),this,SLOT(receiveMesfromc(MySocket*,QByteArray)));
    sendMestoc(socket,COMM_SERVER_CONN_SUCCESSFUL,"服务器:"+socket->peerAddress().toString()+"连接成功");
    emit sendupdatenum(curconnum);
    waits(1500);
    emit sendupdateGameInfo(this);
}


void Server::sendMestoc(MySocket*socket,comm_request_type ty, QString toc)
{
    toc.prepend(QString::number(int(ty))+"##");
    socket->write(toc.toUtf8(),toc.toUtf8().length()+1);
}

void Server::receiveMesfromc(MySocket *socket,QByteArray arr)
{
    forever{
            int i=QString(arr.data()).section("##",0,0).toInt();
            QString s=QString(arr.data()).section("##",1,1);
            int index=0;
            switch (i) {
            case 6://客户端请求创建主机
            {
                QString text=QString(arr.data()).section("##",2,2);
                emit openroom(s,text);//发送开房信号
            }
                return;
            case 7://客户端请求加入房间
            {
                index=playerFightInfo.indexOf(qMakePair<QString,QString>(s,"-"));
                if(index!=-1){
                    playerFightInfo[index].second=QString(arr.data()).section("##",2,2);
                    emit sendupdateGameInfo(this);
                    if(s==mysocket->ip){
                        mysocket->game=new Gamemodel(w,0);//在主窗口中建立线程游戏
                        mysocket->game->type=MM;
                        connect(mysocket->game->c,SIGNAL(sendmesschat(QString)),this,SLOT(sendmesschat(QString)));
                        connect(this,SIGNAL(updatechat(QString)),mysocket->game->c,SLOT(updatechat(QString)));

                        mysocket->game->AItype=none;
                        //bool,Gamemodel *game=0, QObject *parent=0 ,  QString name="Player"
                        mysocket->game->player1=new GPlayer(1,mysocket->game,mysocket->game,s);
                        mysocket->game->player2=new GPlayer(0,mysocket->game,mysocket->game,socket->ip);
                        socket->game=mysocket->game;
                        mysocket->match=socket;
                        socket->match=mysocket;
                        socket->my=mysocket->game->player2;
                        mysocket->game->start();
                        waits(1000);
                        sendMestoc(socket,COMM_SERVER_GAMESTART,"");
                    }//加入服务器房间
                    else
                    {
                        for(int j=0;j<clientlist.size();j++){
                            if(clientlist.at(j)->ip==playerFightInfo.at(index).first)
                                clientlist.at(j)->match=socket;
                                socket->match=clientlist.at(j);//建立客户端之间的匹配连接
                        }
                        Gamemodel *game=new Gamemodel(w,1);//在主窗口中建立线程游戏
                        game->type=MM;
                        game->AItype=none;
                        //bool,Gamemodel *game=0, QObject *parent=0 ,  QString name="Player"
                        game->player1=new GPlayer(1,game,game,socket->match->ip);
                        game->player2=new GPlayer(0,game,game,socket->ip);
                        socket->match->game=game;
                        socket->game=game;
                        socket->match->my=game->player1;
                        socket->my=game->player2;
                        game->start();
                        sendMestoc(socket,COMM_SERVER_GAMESTART,"");
                        sendMestoc(socket->match,COMM_SERVER_GAMESTART,"");
                    }//加入客户端房间
                }
            }
                return;
//            case 8://对方退出游戏           （似乎不需要，在disconnect中实现）
//                    QMessageBox::information(NULL,"游戏停止","对方退出游戏");
//                    socket->game->deleteLater();
//                    socket->game=0;
//                    socket->my=0;
//                    if(!playerFightInfo.removeOne(QPair<QString,QString>(socket->ip,socket->match->ip)))
//                        playerFightInfo.removeOne(QPair<QString,QString>(socket->match->ip,socket->ip));
//                socket->close();
//                socket=0;
//                emit sendupdateGameInfo(this);
//                return;
            case 9://玩家游戏操作：落子
                for(int i=0;i<rowline;i++){
                    for(int j=0;j<columnline;j++){
                        socket->game->game_progress[i][j]=((what)QString(s[i*rowline+j]).toInt());
                    }
                }
                if(socket->game->c!=0) {
                    socket->game->c->update();
//                    waits(500);
                    emit isokon();
                }
                else sendMestoc(socket->match,COMM_CLIENT_GAMEOP,QString(arr.data()));//发送新棋盘消息给对家

                return;
            case 10://玩家游戏操作：悔棋
                if(mysocket==socket->match)
                {
                    QMessageBox:: StandardButton result= QMessageBox::information(NULL,"对方发来信息","对方请求悔棋",QMessageBox::Yes|QMessageBox::No);
                    if(result==QMessageBox::Yes)
                        sendMestoc(socket,COMM_CLIENT_UNDO_YES,"");
                    else
                        sendMestoc(socket,COMM_CLIENT_UNDO_NO,"");
                }
                else
                      sendMestoc(socket->match,COMM_CLIENT_UNDO,"对方请求悔棋，是否同意");
                return;
            case 11://玩家游戏操作：悔棋回复yes
            {
               if(mysocket==socket->match){
                    QMessageBox::information(NULL,"请求成功","对方同意了你的悔棋请求");

               }
               else
               {
                   sendMestoc(socket->match,COMM_CLIENT_UNDO_YES,"对方同意你的悔棋请求");
               }
                socket->game->backStep(socket->match->my);
                QString progress="";
                for(int i=0;i<rowline;i++){
                    for(int j=0;j<columnline;j++){
                        progress+=QString::number(int(socket->game->game_progress[i][j]));
                    }
                }
                waits(1000);
                sendMestoc(socket,COMM_CLIENT_GAMEOP,progress);
                if(mysocket!=socket->match)
                    sendMestoc(socket->match,COMM_CLIENT_GAMEOP,progress);
            }
                return;
            case 12://玩家游戏操作：悔棋回复no
                if(mysocket==socket->match)
                    QMessageBox::information(NULL,"请求失败","对方拒绝了你的悔棋请求,请继续游戏");
                else
                    sendMestoc(socket->match,COMM_CLIENT_UNDO_NO,"对方拒绝了你的悔棋请求,请继续游戏");
                return;
            case 13://玩家游戏操作：认输
                sendMestoc(socket->match,COMM_CLIENT_LOSE,"对方认输，你赢了");
                socket->match->clear();
                if(socket->match==mysocket)
                {
                    delete mysocket;
                    mysocket=0;
                }
                socket->clear(1);
                return;
            case 14://玩家发来ip地址
                socket->ip=s;
                return;
            case 15://玩家游戏操作：发送聊天信息
                if(mysocket==socket->match)
                    emit updatechat(s);
                else
                    sendMestoc(socket->match,COMM_CLIENT_CREATE,s);
                return;
            case 16://玩家发送胜利，游戏结束
                clearroom(socket);
                waits(1000);
                sendMestoc(socket,COMM_SERVER_GAMEOVER,s);
                if(socket->match==mysocket)
                {
                    QString xy=QString(arr.data()).section("##",2,2);
                    mysocket->game->GameEnd(xy.section("//",0,0).toInt(),xy.section("//",1,1).toInt());

                    QMessageBox::information(NULL,"游戏结束",s);
                }
                else
                    sendMestoc(socket->match,COMM_SERVER_GAMEOVER,s);

                socket->match->clear();
                if(socket->match==mysocket){delete mysocket;mysocket=0;}
                socket->clear(1);
                //清空玩家双方套接字，不断开连接
                return;
            default:
                return;
            }
    }
}

void Server::sendmesschat(QString mes)
{
    sendMestoc(mysocket->match,COMM_CLIENT_CHAT,mes);
}

void Server::clearroom(MySocket *x1)
{
    int index=playerFightInfo.indexOf(QPair<QString,QString>(x1->ip,x1->match->ip));
    if(index!=-1)
        playerFightInfo.removeAt(index);
    else {
        int i=playerFightInfo.indexOf(QPair<QString,QString>(x1->match->ip,x1->ip));
        if(i!=-1)
            playerFightInfo.removeAt(i);
    }
    emit sendupdateGameInfo(this);
}

void Server::clearroom(QString ip)
{
    int index=playerFightInfo.indexOf(QPair<QString,QString>(ip,"-"));
    if(index!=-1)
        playerFightInfo.removeAt(index);
    emit sendupdateGameInfo(this);
}

#include "server.h"
QString Server::apppath="";
MySocket *Server::mysocket=0;
Server::Server(QObject *parent,QMainWindow*w) : QTcpServer(parent),w(w)//构造函数，开启服务器
{
    QTime time= QTime::currentTime();
    qsrand(time.msec()+time.second()*3000);//设置随机种子
    while (1) {
        serverdk=qrand();//随机生成服务器端口
        if(listen(QHostAddress::Any,serverdk))
            //开启端口服务
        {
            QClipboard *clipboard = QApplication::clipboard();//获取系统剪贴板指针
            clipboard->setText(QString::number(serverdk));//设置剪贴板内容
            QMessageBox::information(NULL,"成功开启服务器","当前服务端口为"+QString::number(serverdk)+",已复制到剪贴板",QMessageBox::Yes);
            break;//成功退出循环，否则继续生成新端口
        }
    }
    connect(this,&Server::sendupdateGameInfo,this,[&](){
        QString data=QString::number(playerFightInfo.size());
        for(int i=0; i<playerFightInfo.size(); i++)
        data +="##"+playerFightInfo[i].first + " PK " + playerFightInfo[i].second;
        for(int i=0; i<clientlist.size(); i++)
        {
        sendMestoc(clientlist.at(i),COMM_SERVER_GAMEINFO, data);
        }
    },Qt::QueuedConnection);//向每个连接客户端发送最新游戏大厅数据
    mysocket=new MySocket(this);//创建服务器套接字
    mysocket->ip=IP;
}

void Server::waits(int s)//事件循环等待时间
{
        QEventLoop loop;
        QTimer::singleShot(s,&loop,SLOT(quit()));
        loop.exec();
}

Server::~Server()
{

}

void Server::sendpixtoc(MySocket *ss)
{
    waits(100);
    QBuffer buffer;
    buffer.open(QIODevice::ReadWrite);
    ss->Pix.save(&buffer,"jpg");
    quint32 pix_len = (quint32)buffer.data().size();
    quint32  write_len=0;
    sendMestoc(ss->match,COMM_CLIENT_HEAD,"");
    while(write_len<pix_len)
    {
        write_len+= ss->match->write(buffer.data());
    }
    ss->match->waitForBytesWritten();
}



void Server::incomingConnection(qintptr socketDescriptor)//当有服务器连接进来时执行的函数，创建套接字连接，建立客户端断开信号与相关槽函数连接，刷新客户端信息
{
    MySocket*socket=new MySocket(this);//接收，为此客户端创建新套接字
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
        if(isaccord) return;//如果服务器关闭，则直接返回
        curconnum--;
       emit sendupdatenum(curconnum);
        int n=clientlist.size();
        for(int i=0; i<n;i++) {
            if(clientlist[i]->write("-1")==-1){
                if(clientlist.at(i)->match!=0&&clientlist.at(i)->match!=mysocket)
                    sendMestoc( clientlist.at(i)->match,COMM_CLIENT_QUITGAME,"对方退出游戏");//如果玩家在游戏中，给对手发送玩家退出游戏信息
                else
                {
                    emit sendupdatemesbox(clientlist.at(i)->ip+"断开服务器");
                    if(clientlist.at(i)->match!=0)
                    {
                        clientlist.at(i)->match->game->stop();
                        delete clientlist.at(i)->match->game;
                        clientlist.at(i)->match->game=0;
                        clearroom(clientlist.at(i));
                        clientlist.at(i)->clear(1);
                        QMessageBox::information(NULL,"游戏停止","对方退出游戏");
                    }//停止游戏进程，清空房间和对手套接字，不断开连接
                    else{
                        clearroom(clientlist.at(i)->ip);
                    }
                }
                clientlist.removeAt(i);//从客户端列表移除
            }
        }
    });//断开连接,当前连接数减一
    clientlist.append(socket);//添加至已连接客户端列表
    sendMestoc(socket,COMM_SERVER_CONN_SUCCESSFUL,"服务器:"+socket->peerAddress().toString()+"连接成功");//发送连接成功消息给客户端
    emit sendupdatenum(curconnum);//改变连接人数
    emit sendupdateGameInfo(this);//改变房间信息

}


void Server::sendMestoc(MySocket*socket,comm_request_type ty, QString toc)//对信息进行包装，发送给客户端的函数
{
    waits(100);
    toc.prepend(QString::number(int(ty))+"##");
    if(ty!=COMM_CLIENT_HEAD)toc.append("&&");
    socket->write(toc.toUtf8(),toc.toUtf8().length()+1);
}
#define GETMES(n) ss.section("##",n,n)
void Server::receiveMesfromc(MySocket *socket,QByteArray arr)//接收来自客户端的信息并处理
{
    QString sum=QString(arr.data());
    QStringList meslist;
    int n=0;
    while(!sum.section("&&",n,n).isEmpty()){
        meslist<<sum.section("&&",n,n);
        n++;
    }
    for(int k=0;k<meslist.size();k++){
        QString ss=meslist.at(k);
        int i=ss.section("##",0,0).toInt();//获得信息类型
        QString s=ss.section("##",1,1);//获得信息内容
            int index=0;
            switch (i) {
            case 6://客户端请求创建主机
            {
                emit openroom(s,GETMES(2));//发送开房信号
            }
                break;
            case 7://客户端请求加入房间
            {
                index=playerFightInfo.indexOf(qMakePair<QString,QString>(s,"-"));//找到房间下标
                if(index!=-1){
                    playerFightInfo[index].second=GETMES(2);//更改房间信息
                    emit sendupdateGameInfo(this);
                    if(s==mysocket->ip){//加入服务器房间
                        emit sendmes(socket->Pix,socket->pername);
                        mysocket->game=new Gamemodel(this,0);//建立线程游戏
                        mysocket->game->type=MM;  mysocket->game->AItype=none;
                        connect(mysocket->game->c,SIGNAL(sendmesschat(QString)),this,SLOT(sendmesschat(QString)),Qt::QueuedConnection);
                        connect(this,SIGNAL(updatechat(QString)),mysocket->game->c,SLOT(updatechat(QString)),Qt::QueuedConnection);
                        connect(mysocket->game,&Gamemodel::sendback,this,[&](){
                            sendMestoc(mysocket->match,COMM_CLIENT_UNDO,"对方请求悔棋，是否同意");
                        },Qt::QueuedConnection);
                        connect(mysocket->game,&Gamemodel::sendgv,this,[&](){
                            sendMestoc(mysocket->match,COMM_CLIENT_LOSE,"对方认输，你赢了");
                            mysocket->match->clear();
                            clearroom(mysocket);
                            mysocket->clear(1);
                        },Qt::QueuedConnection);
                        emit sendsetmes(mysocket->Pix,mysocket->pername,socket->Pix,socket->pername);
                        //设置玩家双方套接字
                        //bool,Gamemodel *game=0, QObject *parent=0 ,  QString name="Player"
                        mysocket->game->player1=new GPlayer(1,mysocket->game,mysocket->game,mysocket->pername);
                        mysocket->game->player2=new GPlayer(0,mysocket->game,mysocket->game,socket->pername);
                        mysocket->game->c->setbegin();
                        socket->game=mysocket->game;
                        mysocket->match=socket;
                        socket->match=mysocket;
                        mysocket->my=mysocket->game->player1;
                        socket->my=mysocket->game->player2;
                        sendpixtoc();//发送图片
                        sendMestoc(socket,COMM_SERVER_GAMESTART,mysocket->pername);//玩家发送游戏开始信息
                        mysocket->game->start();
                    }
                    else
                    {//加入客户端房间
                        for(int j=0;j<clientlist.size();j++){
                            if(clientlist.at(j)->ip==playerFightInfo.at(index).first)
                            {
                                clientlist.at(j)->match=socket;
                                socket->match=clientlist.at(j);//建立客户端之间的匹配连接
                                break;
                            }
                        }
                        Gamemodel *game=new Gamemodel(this,1);//建立线程游戏
                        game->type=MM;
                        game->AItype=none;
                        //bool,Gamemodel *game=0, QObject *parent=0 ,  QString name="Player"
                        game->player1=new GPlayer(1,game,game,socket->match->pername);
                        game->player2=new GPlayer(0,game,game,socket->pername);
                        socket->match->game=game;
                        socket->game=game;
                        socket->match->my=game->player1;
                        socket->my=game->player2;
                        sendpixtoc(socket);
                        sendpixtoc(socket->match);
                        sendMestoc(socket,COMM_SERVER_GAMESTART,socket->match->pername);
                        sendMestoc(socket->match,COMM_SERVER_GAMESTART,socket->pername);
                        game->start();

                    }
                }
            }
                break;
            case 8://对方刷新游戏大厅
                emit sendupdateGameInfo(this);
                break;
            case 9://玩家游戏操作：落子
                for(int i=0;i<rowline;i++){
                    for(int j=0;j<columnline;j++){
                        socket->game->game_progress[i][j]=((what)QString(s[i*rowline+j]).toInt());
                    }
                }//读取最新棋盘信息
                if(!GETMES(2).isEmpty()&&GETMES(2).toInt()>=0&&GETMES(3).toInt()>=0)//获得上一步落子坐标
                {
                    socket->game->black_score[GETMES(2).toInt()][GETMES(3).toInt()]=-2000;
                    socket->game->white_score[GETMES(2).toInt()][GETMES(3).toInt()]=-2000;
                    socket->my->backx=GETMES(2).toInt();socket->my->backy=GETMES(3).toInt();
                    socket->game->backx=GETMES(2).toInt();socket->game->backy=GETMES(3).toInt();
                    qDebug()<<socket->game->backx<<" "<<socket->game->backy<<"gameop";
                    if(socket->game->player2->myflag){
                        socket->game->player2->calculatBlackUp(socket->my->backx,socket->my->backy);
                    }
                    else{
                        socket->game->player2->calculatWhiteUp(socket->my->backx,socket->my->backy);
                    }
                }
                if(socket->game->c!=0) {
                    socket->game->c->update();
                    emit isokon();//发送可以落子信号
                }
                else sendMestoc(socket->match,COMM_CLIENT_GAMEOP,s+"##"+GETMES(2)+"##"+GETMES(3));//发送新棋盘消息给对家
                break;
            case 10://玩家游戏操作：悔棋
            {   
                if(socket->match==mysocket)//服务器与客户端
                {
                    if(mysocket->game->isdeposit){
                        sendMestoc(socket,COMM_CLIENT_UNDO_NO,"对方托管中，拒绝了你的悔棋请求,请继续游戏");
                    }else
                    {
                        QMessageBox:: StandardButton result= QMessageBox::information(NULL,"对方发来信息","对方请求悔棋",QMessageBox::Yes|QMessageBox::No);
                        if(result==QMessageBox::Yes)
                        {
                            sendMestoc(socket,COMM_CLIENT_UNDO_YES,"对方同意你的悔棋请求");
                            QString progress="";
                            for(int i=0;i<rowline;i++){
                                for(int j=0;j<columnline;j++){
                                    progress+=QString::number(int(mysocket->game->game_progress[i][j]));
                                }
                            }
                            sendMestoc(socket,COMM_CLIENT_GAMEOP,progress);

                        }
                        else
                            sendMestoc(socket,COMM_CLIENT_UNDO_NO,"对方拒绝了你的悔棋请求,请继续游戏");
                    }
                }
                else//客户端与客户端
                    sendMestoc(socket->match,COMM_CLIENT_UNDO,"对方请求悔棋，是否同意");
            }
                break;
            case 11://玩家游戏操作：悔棋回复yes
            {

                socket->game->backStep(socket->match->my);
                QString progress="";
                for(int i=0;i<rowline;i++){
                    for(int j=0;j<columnline;j++){
                        progress+=QString::number(int(socket->game->game_progress[i][j]));
                    }
                }
                sendMestoc(socket,COMM_CLIENT_GAMEOP,progress);//发送悔棋后棋盘信息
                if(mysocket!=socket->match)
                    sendMestoc(socket->match,COMM_CLIENT_GAMEOP,progress);
                if(mysocket==socket->match){//服务器与客户端
                  QMessageBox::information(NULL,"请求成功","对方同意了你的悔棋请求");
                }
                else//客户端与客户端
                {
                    sendMestoc(socket->match,COMM_CLIENT_UNDO_YES,"对方同意你的悔棋请求");
                }
            }
                break;
            case 12://玩家游戏操作：悔棋回复no
                if(mysocket==socket->match)
                    QMessageBox::information(NULL,"请求失败","对方拒绝了你的悔棋请求,请继续游戏");
                else
                    sendMestoc(socket->match,COMM_CLIENT_UNDO_NO,"对方拒绝了你的悔棋请求,请继续游戏");
                break;
            case 13://玩家游戏操作：认输
                if(socket->match==mysocket)
                    QMessageBox::information(NULL,"游戏结束","对方认输，你赢了");
                else
                    sendMestoc(socket->match,COMM_CLIENT_LOSE,"对方认输，你赢了");
                socket->match->clear();
                clearroom(socket);
                socket->clear(1);//清空套接字，清空房间
                break;
            case 14://玩家发来ip地址和用户名
                socket->ip=s;
                socket->pername=GETMES(2);
                emit sendupdatemesbox(socket->ip+"连接服务器。");//发送客户端连接信息给消息框和小助手
                break;
            case 15://玩家游戏操作：发送聊天信息
                if(socket->match==mysocket)
                    emit updatechat(s);
                else
                    sendMestoc(socket->match,COMM_CLIENT_CHAT,s);
                break;
            case 16://玩家发送胜利，游戏结束
                clearroom(socket);
                sendMestoc(socket,COMM_SERVER_GAMEOVER,s);
                if(socket->match==mysocket)
                {
                    if(!GETMES(2).isEmpty()&&GETMES(2)>=0&&GETMES(3)>=0)
                        mysocket->game->state=mysocket->game->GameEnd(GETMES(2).toInt(),GETMES(3).toInt());
                    QMessageBox::information(NULL,"游戏结束",s);
                }
                else
                    sendMestoc(socket->match,COMM_SERVER_GAMEOVER,s);
                socket->match->clear();
                socket->clear(1);
                //清空玩家双方套接字，不断开连接
                break;
            case 18://玩家发送图片
            {
                QByteArray array=arr.mid(4);
                QBuffer buffer(&array);
                buffer.open(QIODevice::ReadOnly);
                QImageReader reader(&buffer,"JPG");
                QImage img(reader.read());
                if(!img.isNull()){
                    socket->Pix=QPixmap::fromImage(img);
                }
                else {
                    socket->Pix=QPixmap(":/new/myresouce/reso/head-portrait/tx1.jpg");//接收图片失败，使用默认图片
                }
            }
                break;
            default:
                break;
            }
    }
}

void Server::sendmesschat(QString mes)
{
    sendMestoc(mysocket->match,COMM_CLIENT_CHAT,mes);
}

void Server::clearroom(MySocket *x1)//根据套接字，清空游戏结束的房间
{
    int index=playerFightInfo.indexOf(QPair<QString,QString>(x1->ip,x1->match->ip));//寻找房间下标
    if(index!=-1)
        playerFightInfo.removeAt(index);
    else {
        int i=playerFightInfo.indexOf(QPair<QString,QString>(x1->match->ip,x1->ip));
        if(i!=-1)
            playerFightInfo.removeAt(i);
        else
            return;
    }
    emit sendupdateGameInfo(this);
}

void Server::clearroom(QString ip)//根据ip，清空游戏结束的房间
{
    int index=playerFightInfo.indexOf(QPair<QString,QString>(ip,"-"));//寻找房间下标
    if(index!=-1)
    {
        playerFightInfo.removeAt(index);
        emit sendupdateGameInfo(this);
    }
}

void Server::GameOver()//联机游戏结束
{
    mysocket->game->player1->ontime->stop();
    if(mysocket->game->state==win)
    {
        if(mysocket->game->player1->istimeover)
        {
            sendMestoc(mysocket->match,COMM_SERVER_GAMEOVER,mysocket->match->pername+"胜利##"+QString::number(mysocket->game->backx)+"##"+QString::number(mysocket->game->backy));
            QMessageBox::information(NULL,"游戏结束",mysocket->game->player1->name+"超时，"+mysocket->match->pername+"胜利");
        }
        else
        {
            sendMestoc(mysocket->match,COMM_SERVER_GAMEOVER,mysocket->game->player1->name+"胜利##"+QString::number(mysocket->game->backx)+"##"+QString::number(mysocket->game->backy));
            QMessageBox::information(NULL,"游戏结束",mysocket->game->player1->name+"胜利");
        }
    }
    else
        {
            sendMestoc(mysocket->match,COMM_SERVER_GAMEOVER,"双方和棋##"+QString::number(mysocket->game->backx)+"##"+QString::number(mysocket->game->backy));
            QMessageBox::information(NULL,"游戏结束","双方和棋");
        }
    mysocket->match->clear();
    clearroom(mysocket);
    mysocket->clear(1);
    //清空玩家双方套接字，不断开连接
}
void Server::receiveprogress(QString p)
{
    sendMestoc(mysocket->match,COMM_CLIENT_GAMEOP,p);
}

#include "client.h"
#include<Qtnetwork>
QString Client::apppath="";
Client::Client(QString dk, QString ip, QObject *parent) : QObject(parent)
{
    iscon=0;
    socket=new MySocket(this);
    socket->connectToHost(QHostAddress(ip),dk.toUInt());//连接服务器
    if(socket->waitForConnected()){ iscon=1;}//判断连接是否成功
    else{
        return;
    }
    connect(socket,&MySocket::readyRead,this,[&](){
        socket->mes.append(socket->readAll());
    },Qt::QueuedConnection);
}

void Client::sendMessagetos(comm_request_type ty,QString toc)
{
            waits(100);
            toc.prepend(QString::number(int(ty))+"##");
            if(ty!=COMM_CLIENT_HEAD)toc.append("&&");
            socket->write(toc.toUtf8());
}

Client::~Client()
{

    if(game!=0)//如果游戏还在进行，停止游戏
    {
        game->stop();
        delete game;
        game=0;
    }
    if(socket!=nullptr){//释放套接字内存
        delete socket;
        socket=nullptr;
    }
}

void Client::sendpixtos()
{
    waits(100);
    QBuffer buffer;
    buffer.open(QIODevice::ReadWrite);
    socket->Pix.save(&buffer,"jpg");
    quint32 pix_len = (quint32)buffer.data().size();
    quint32  write_len=0;
    sendMessagetos(COMM_CLIENT_HEAD,"");
    while (write_len<pix_len) {
        write_len+= (quint32)socket->write(buffer.data());
    }
}

void Client::waits(int n)
{
    QEventLoop loop;
    QTimer::singleShot(n,&loop,SLOT(quit()));
    loop.exec();
}



void Client::sendMesschat(QString mes)
{
    sendMessagetos(COMM_CLIENT_CHAT, mes);
}

#include "client.h"
#include<Qtnetwork>
Client::Client(QString dk, QString ip, QObject *parent) : QObject(parent)
{
    iscon=0;
    socket=new MySocket(this);
    socket->connectToHost(QHostAddress(ip),dk.toUInt());
    if(socket->waitForConnected()){ iscon=1;}
    else{
        QMessageBox::information(NULL,"连接失败","服务器连接失败（可能无访问权限或该端口服务器不存在）");
    }
    connect(socket,&MySocket::readyRead,this,[&](){
        emit socket->send(socket->read(socket->bytesAvailable()));
    });


}

Client::~Client()
{

    if(game!=0)
    {
        game->stop();
        delete game;
    }
    if(socket!=0){
        delete socket;}
}

void Client::sendMessagetos(comm_request_type type,QString toc)
{
    if(sendtimelast.isNull())
    {
        sendtimelast=QTime::currentTime();
    }
    else
    {
        int cha=sendtimelast.msecsTo(QTime::currentTime());
        if(cha<1000)
        {
            QEventLoop loop;
            QTimer::singleShot(1000-cha,&loop,SLOT(quit()));
            loop.exec();
        }
    }
            toc.prepend(QString::number(int(type))+"##");
            socket->write(toc.toUtf8());
            sendtimelast=QTime::currentTime();
}

void Client::sendMesschat(QString mes)
{
    sendMessagetos(COMM_CLIENT_CHAT, mes);
}

#include "client.h"

Client::Client(QString dk, QString ip, QObject *parent) : QObject(parent)
{
    iscon=0;
    socket=new QTcpSocket(this);
    socket->connectToHost(QHostAddress(ip),dk.toUInt());
    if(socket->waitForConnected()){ iscon=1;}
    else{
        QMessageBox::information(NULL,"连接失败","服务器连接失败（可能无访问权限或该端口服务器不存在）");
    }
    connect(socket,SIGNAL(readyRead()),this->parent(),SLOT(receiveMessage()));
}

Client::~Client()
{
    if(game!=0)
    {
        game->stop();
        delete game;
    }
    if(socket!=0)delete socket;
}


void Client::sendMessagetos(comm_request_type type,QString toc)
{
    toc.prepend(QString::number(int(type))+"##");
    socket->write(toc.toUtf8());
}

void Client::sendMesschat(QString mes)
{
    sendMessagetos(COMM_CLIENT_CHAT, mes);
}

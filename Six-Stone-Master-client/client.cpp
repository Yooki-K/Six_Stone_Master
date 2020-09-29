#include "client.h"
#include<Qtnetwork>
QString Client::apppath="";
Client::Client(QString dk, QString ip, QObject *parent) : QObject(parent)
{
    iscon=0;
    socket=new MySocket(this);
    socket->connectToHost(QHostAddress(ip),dk.toUInt());
    if(socket->waitForConnected()){ iscon=1;}
    else{
        return;
    }
    connect(socket,&MySocket::readyRead,this,[&](){
        emit socket->send(socket->read(socket->bytesAvailable()));
    },Qt::QueuedConnection);

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

void Client::sendpixtos()
{
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

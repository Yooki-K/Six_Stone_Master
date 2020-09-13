#include "client.h"

Client::Client(QString dk, QString ip, QObject *parent) : QObject(parent)
{
    socket=new QTcpSocket;
    socket->connectToHost(QHostAddress(ip),dk.toUInt());
    connect(socket,SIGNAL(readyRead()),this,SLOT(receiveMessage()));
}

void Client::receiveMessage()
{
    if(socket->bytesAvailable()>0){//socket的有效字节
//        socket->read(inc,socket->bytesAvailable());
        QByteArray arr=socket->readAll();
        qDebug()<<arr.data();
    }

}
void Client::sendMessage(QString toc)
{
    socket->write(toc.toUtf8());
}

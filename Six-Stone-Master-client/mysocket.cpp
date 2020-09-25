#include "mysocket.h"

MySocket::MySocket(QObject *parent):QTcpSocket(parent)
{
    connect(this,SIGNAL(send(QByteArray)),this->parent()->parent(),SLOT(receiveMessage(QByteArray)));
}


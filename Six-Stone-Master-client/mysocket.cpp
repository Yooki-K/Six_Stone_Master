#include "mysocket.h"

MySocket::MySocket(QObject *parent):QTcpSocket(parent)
{
    connect(this,SIGNAL(send(QByteArray)),this->parent()->parent(),SLOT(receiveMessage(QByteArray)),Qt::QueuedConnection);

    QtConcurrent::run([&](){
        while (!isover) {
            QEventLoop loop;
            QTimer::singleShot(100,&loop,SLOT(quit()));
            loop.exec();
            if(isover)
                break;
            if(!mes.isNull()){
                emit send(mes);
                qDebug()<<mes.data();
                mes.clear();
            }
        }
        isreallyover=1;
    });
}

MySocket::~MySocket()
{
    while (!isreallyover) {
        isover=1;
        QEventLoop loop;
        QTimer::singleShot(500,&loop,SLOT(quit()));
        loop.exec();
    }

}






#include "mysocket.h"

MySocket::MySocket(QObject *parent):QTcpSocket(parent)
{
    connect(this,SIGNAL(send(MySocket*,QByteArray)),this->parent(),SLOT(receiveMesfromc(MySocket*,QByteArray)));
    connect(this,&MySocket::readyRead,this,[&](){
        mes.append(readAll());
    });
    QtConcurrent::run([&](){
        Sender sen;
        bool isover=0;
        connect(this,&MySocket::destroyed,&sen,[&](){
            isover=1;
        },Qt::QueuedConnection);
        while (!isover) {
            QEventLoop loop;
            QTimer::singleShot(100,&loop,SLOT(quit()));
            connect(this,&MySocket::destroyed,&loop,&QEventLoop::quit);
            loop.exec();
            if(isover) break;
            if(!mes.isNull()){
                emit send(this,mes);
                mes.clear();
            }
        }
    });
}

//MySocket*match=0;
//Gamemodel *game=0;
//GPlayer*my=0;
//QString ip="";
void MySocket::clear(int n)//清空套接字函数
{// 0 全部清空// 1 全部清空，游戏指针除外
    if(this==0) return;
    if(n==0){
        match=0;
        if(game!=0)
        {
            game->stop();
            delete game;
            game=0;
        }
        my=0;
     }
    else{
        match=0;
        game=0;
        my=0;
    }
}






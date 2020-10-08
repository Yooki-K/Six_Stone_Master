#include "mysocket.h"

MySocket::MySocket(QObject *parent):QTcpSocket(parent)
{

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

MySocket::~MySocket()
{

}



void MySocket::receiveMesfromc()
{
        if(this->bytesAvailable()>0)
            emit send(this,this->readAll());//发送信息给处理信息的函数
}

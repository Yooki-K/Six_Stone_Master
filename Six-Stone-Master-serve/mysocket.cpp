#include "mysocket.h"

MySocket::MySocket(QObject *parent):QTcpSocket(parent)
{

}

//MySocket*match=0;
//Gamemodel *game=0;
//GPlayer*my=0;
//QString ip="";
void MySocket::clear(int n)// 0 all// 1 some
{
    if(n==0){
        match=0;
        game->stop();
        delete game;
        game=0;
        my=0;
     }
    else{
        match=0;
        game=0;
        my=0;
    }
}

void MySocket::receiveMesfromc()
{
    if(this->bytesAvailable()>0)//client->socket的有效字节
    emit send(this,this->readAll());
}

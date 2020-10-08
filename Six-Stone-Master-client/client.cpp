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
        emit socket->send(socket->read(socket->bytesAvailable()));//套接字接收消息，处理信号
    },Qt::QueuedConnection);

}

Client::~Client()
{

    if(game!=0)//如果游戏还在进行，停止游戏
    {
        game->stop();
        delete game;
        game=0;
    }
    if(socket!=0){//释放套接字内存
        delete socket;
        socket=0;
    }
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

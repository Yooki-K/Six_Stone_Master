#include "server.h"

Server::Server(QObject *parent) : QTcpServer(parent)
{
    connect(this,SIGNAL(sendmess(QString)),this->parent(),SLOT(updatelabel(QString)));
    QTime time= QTime::currentTime();
    qsrand(time.msec()+time.second()*3000);
    while (1) {
        serverdk=qrand();
        if(listen(QHostAddress::Any,serverdk))
            //开启端口服务
        {
            curconnum++;
            QClipboard *clipboard = QApplication::clipboard();//获取系统剪贴板指针
            clipboard->setText(QString::number(serverdk));//设置剪贴板内容</span>
            QMessageBox::information(NULL,"成功开启服务器","当前服务端口为"+QString::number(serverdk)+",已复制到剪贴板",QMessageBox::Yes);
            emit sendmess(QString::number(serverdk));
            break;
        }
    }
}
//当有服务器连接进来时执行的函数
void Server::incomingConnection(qintptr handle)
{
    socket=new QTcpSocket(this);
    socket->setSocketDescriptor(handle);//建立连接
    connect(socket,SIGNAL(readyRead()),this,SLOT(receiveMessage()));
}

void Server::receiveMessage()
{
    if(socket->bytesAvailable()>0){//socket的有效字节
        socket->read(inc,socket->bytesAvailable());
    }
}

void Server::sendMessage(QString toc)
{
    socket->write(toc.toUtf8());
}

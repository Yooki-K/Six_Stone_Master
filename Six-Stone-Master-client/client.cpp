#include "client.h"

Client::Client(QString dk, QString ip, QObject *parent) : QObject(parent)
{
    socket=new QTcpSocket;
    socket->connectToHost(QHostAddress(ip),dk.toUInt());
    connect(socket,SIGNAL(readyRead()),this->parent(),SLOT(receiveMessage()));
    connect(this,SIGNAL(updateroom(QString)),this->parent(),SLOT(updateroom(QString)));
   //获得本机ip地址
      QList<QHostAddress> AddressList = QNetworkInterface::allAddresses();
      foreach(QHostAddress address, AddressList){
          if(address.protocol() == QAbstractSocket::IPv4Protocol &&
             address != QHostAddress::Null &&
             address != QHostAddress::LocalHost){
              if (address.toString().contains("127.0.")){
                continue;
              }
              myip = address.toString();
              break;
          }
      }
}


void Client::sendMessagetos(comm_request_type type,QString toc)
{
    toc.prepend(QString::number(int(type))+"##");
    socket->write(toc.toUtf8());
}

void Client::sendMesschat(QString mes)
{
    sendMessagetos(COMM_CLIENT_CHAT, game->player1->name+':'+mes);
}

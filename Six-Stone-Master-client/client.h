#ifndef CLIENT_H
#define CLIENT_H
#include<QHostInfo>
#include <QObject>
#include<QTcpSocket>
#include<QByteArray>
#include<QDebug>
#include<QTimer>
#include<QHostAddress>
#include"database.hpp"
#include"gamemodel.h"
#include"chessboard.h"
#include"gplayer.h"
#include"mysocket.h"
#include<QNetworkInterface>
class Client : public QObject
{
    Q_OBJECT
private:

public:
    explicit Client(QString,QString,QObject *parent = nullptr);
    ~Client();
    MySocket *socket=0;
    Gamemodel *game=0;
    bool myflag;
    bool iscon=0;
    QTime sendtimelast;
signals:

public slots:
    void sendMessagetos(comm_request_type type, QString toc);
    void sendMesschat(QString);
};


#endif // CLIENT_H

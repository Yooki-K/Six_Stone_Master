#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include<QTcpSocket>
#include<QByteArray>
#include<QDebug>
#include<QHostAddress>
#include"database.hpp"
#include"gamemodel.h"
#include"chessboard.h"
#include"gplayer.h"
#include<QNetworkInterface>
class Client : public QObject
{
    Q_OBJECT
private:

public:
    explicit Client(QString,QString,QObject *parent = nullptr);
    QTcpSocket *socket;
    Gamemodel *game;
    bool myflag;
    QString myip;
signals:
    void updateroom(QString);
public slots:
    void sendMessagetos(comm_request_type type, QString toc);
    void sendMesschat(QString);
};


#endif // CLIENT_H

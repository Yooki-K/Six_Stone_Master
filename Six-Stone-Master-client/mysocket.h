#ifndef MYSOCKET_H
#define MYSOCKET_H
#include<QTcpSocket>
#include"gamemodel.h"
#include <QObject>
#include <QtConcurrent>
class Sender : public QObject
{
    Q_OBJECT
public:
    Sender(QObject *parent = nullptr):QObject(parent) {}
signals:
    void isread();
};

class MySocket:public QTcpSocket
{
    Q_OBJECT
public:
    explicit MySocket(QObject *parent =nullptr);
    void clear(int n=0);
    bool isover=0;
    bool isreallyover=0;
    QPixmap Pix;
    QString pername;
    QByteArray mes;
    ~MySocket();
public slots:

signals:
    void send(QByteArray);
    void sendtos();


};

#endif // MYSOCKET_H

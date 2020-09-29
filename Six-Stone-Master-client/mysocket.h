#ifndef MYSOCKET_H
#define MYSOCKET_H
#include<QTcpSocket>
#include"gamemodel.h"
#include <QObject>

class MySocket:public QTcpSocket
{
    Q_OBJECT
public:
    explicit MySocket(QObject *parent =nullptr);
    void clear(int n=0);
    bool isrun=1;
    QPixmap Pix;
    QString pername;
public slots:
signals:
    void send(QByteArray);
};

#endif // MYSOCKET_H

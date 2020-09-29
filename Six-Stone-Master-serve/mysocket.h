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
    MySocket*match=0;
    Gamemodel *game=0;
    QPixmap Pix;
    QString pername;
    GPlayer*my=0;
    QString ip="";
    void clear(int n=0);
signals:
    void send(MySocket *,QByteArray);
public slots:
    void receiveMesfromc();
};

#endif // MYSOCKET_H

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
    GPlayer*my=0;
};

#endif // MYSOCKET_H

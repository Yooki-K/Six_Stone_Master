#ifndef MYSOCKET_H
#define MYSOCKET_H
#include<QTcpSocket>
#include"gamemodel.h"
#include <QObject>
#include<QtConcurrent>
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
    MySocket*match=0;//对手套接字指针
    Gamemodel *game=0;//游戏进程指针
    QPixmap Pix;//头像图片
    QString pername;//用户名
    GPlayer*my=0;//玩家指针
    QString ip="";//ip
    QByteArray mes;
    void clear(int n=0);//对当前套接字进行清空
signals:
    void send(MySocket *,QByteArray);//发送消息信号
public slots:
};

#endif // MYSOCKET_H

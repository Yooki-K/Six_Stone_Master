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
#include"ai.h"
#include"mysocket.h"
#include<QNetworkInterface>
#include<QBuffer>
#include<QImage>
#include<QImageReader>


class Client : public QObject
{
    Q_OBJECT
private:

public:
    explicit Client(QString,QString,QObject *parent = nullptr);
    ~Client();
    MySocket *socket=nullptr;//客户端套接字
    Gamemodel *game=0;//游戏进程指针
    bool myflag;//黑白方（真为黑  假为白）
    static QString apppath;//可执行程序路径
    QPixmap pix;//头像图片
    QString pername;//用户名
    bool iscon=0;//判断是否连接服务器
    QTime sendtimelast;//最后一次发送信息给服务器时间
    void sendpixtos();//发送图片给服务器
    void waits(int);
public slots:
    void sendMessagetos(comm_request_type type, QString toc);//发送信息给服务器
    void sendMesschat(QString);//发送玩家聊天信息
signals:
};


#endif // CLIENT_H

#ifndef SERVER_H
#define SERVER_H
#include<QApplication>
#include<QMessageBox>
#include <QObject>
#include<QDebug>
#include<QTcpServer>
#include <QtGlobal>//包含随机函数
#include<QTime>
#include<QMap>
#include<QByteArray>
#include<QClipboard>//对剪贴板进行操作
#include"database.hpp"
#include"ai.h"
#include"chessboard.h"
#include<QMainWindow>
#include"mysocket.h"
const int maxcurcon=4;
class Server : public QTcpServer
{
    Q_OBJECT
private:
public:
    explicit Server(QObject *parent = nullptr,QMainWindow*w=nullptr);
    quint16 serverdk;
    QMainWindow*w;
    MySocket *mysocket=0;
    int curconnum=0;
    QList<MySocket*>clientlist;
    // 用户程序之间交互的数据结构
    msg_request_struct* msg_req_struct;
    // 记录对弈双方的IP信息，若主机在等待其他玩家加入，则其对手IP地址置为“-”
    QList<QPair<QString, QString> >playerFightInfo;
    void waits(int );
signals:
    void senddktoui(QString);
    void sendupdateGameInfo(Server *);
    void openroom(QString,QString);
    void isokon();
    void sendupdatenum(int);
    void updatechat(QString);
public slots:
   void incomingConnection(qintptr);
   void sendMestoc(MySocket *, comm_request_type, QString);
   void receiveMesfromc(MySocket*,QByteArray);
   void sendmesschat(QString);
   void clearroom(MySocket*);
   void clearroom(QString);

};

#endif // SERVER_H


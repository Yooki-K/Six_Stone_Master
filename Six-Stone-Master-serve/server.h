#ifndef SERVER_H
#define SERVER_H
#include<QApplication>
#include<QMessageBox>
#include <QObject>
#include<QBuffer>
#include<QImage>
#include<QImageReader>
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
#define IP "127.0.0.1"
const int maxcurcon=2;
class Server : public QTcpServer
{
    Q_OBJECT
private:
public:
    explicit Server(QObject *parent = nullptr,QMainWindow*w=nullptr);
    quint16 serverdk;//服务器端口
    bool isaccord=0;//查看是否是服务器主动断开
    static QString apppath;//可执行程序路径
    QMainWindow*w;
    static MySocket *mysocket;//服务器本地套接字
    int curconnum=0;//客户端连接个数
    QList<MySocket*>clientlist;//连接的客户端套接字
    QTime sendtimelast;//最近一次给客户端发送消息的时间
    QList<QPair<QString, QString> >playerFightInfo;// 记录对弈双方的IP信息，若主机在等待其他玩家加入，则其对手IP地址置为“-”
    void waits(int);//时间循环等待函数
    ~Server();
    void sendpixtoc(MySocket *ss=mysocket);//发送玩家头像图片给客户端
signals:
    void sendupdateGameInfo(Server *);//房间发生改动时触发的信号
    void openroom(QString,QString);//开房信号
    void isokon();//可以下棋信号
    void sendupdatenum(int);//连接人数变动时触发的信号
    void updatechat(QString);//玩家聊天时触发的信号
    void sendupdatemesbox(QString);//客户端连接和断开时触发的信号
    void sendsetmes(QPixmap,QString,QPixmap,QString);//发送一个房间两个玩家的个人信息（用户名、头像）
public slots:
   void incomingConnection(qintptr);//当有客户端连接时调用的槽函数
   void sendMestoc(MySocket *, comm_request_type, QString);//给客户端发送信息
   void receiveMesfromc(MySocket*,QByteArray);//接收客户端的信息
   void sendmesschat(QString);//传递玩家聊天信息
   void clearroom(MySocket*);//清空房间
   void clearroom(QString);//清空房间（重载函数）
   void GameOver();//游戏结束
   void receiveprogress(QString);//接收棋盘信息
};

#endif // SERVER_H


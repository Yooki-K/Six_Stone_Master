#ifndef GAMEMODEL_H
#define GAMEMODEL_H
#include<vector>
#include<QMessageBox>
#include<QThread>
#include"database.hpp"
#include<QMainWindow>

#define SPLAYER1 QEventLoop loop;\
connect(this->c,SIGNAL(mouseRelease()),&loop,SLOT(quit()));\
connect(this,SIGNAL(unlock()),&loop,SLOT(quit()));\
loop.exec();\
if(isstop) break;\
player1->myturn(c->clickx,c->clicky);\
QString progress="";\
for(int i=0;i<rowline;i++){\
    for(int j=0;j<columnline;j++){\
        progress+=QString::number(int(game_progress[i][j]));\
    }\
}\
connect(this,SIGNAL(sendprogress(QString)),this->parent()->parent(),SLOT(receiveprogress(QString)));\
emit sendprogress(progress);\
if(isstop) break;\
if(state!=playing) {\
QEventLoop loop;\
QTimer::singleShot(1000,&loop,SLOT(quit()));\
loop.exec();\
emit gameover();\
QEventLoop loopp;\
connect(this,SIGNAL(unlock()),&loopp,SLOT(quit()));\
loopp.exec();\
}\
emit stopt();

#define SPLAYER2 Gameflags=!Gameflags;\
QEventLoop looppp;\
connect(this->parent()->parent(),SIGNAL(isokon()),&looppp,SLOT(quit()));\
connect(this,SIGNAL(unlock()),&looppp,SLOT(quit()));\
looppp.exec();\
emit startt();

class GPlayer;
class Chessboard;
class Gamemodel:public QThread
{
    Q_OBJECT
private:
    bool isstop=0;//是否游戏结束
public:
    GPlayer *player1=0;GPlayer *player2=0;//玩家一、二号
    Gametype type;//游戏模式
    Gamestate state;//游戏状态
    GameAI AItype;//机器人模式
    Chessboard *c=0;//棋盘界面指针
    int winx=0;int winy=0;//游戏胜利时落子的坐标
    int backx=-1;int backy=-1;//最新落子坐标
    bool Gameflags=1;//第一下棋方，黑棋
    std::vector<std::vector<what>> game_progress;//棋盘信息
    int black_score[rowline][columnline];//黑棋得分
    int white_score[rowline][columnline];//白棋得分
    int derect=-1;//六子连线方向
    int isonline=-1;//游戏服务器状态（-1本地端 0服务器和客户端 1客户端和客户端）
    Gamemodel(QObject *parent=nullptr);
    ~Gamemodel();
    void run();//进程运行
    int IsSix(int x,int y);//判断是否六子连线
    Gamestate GameEnd(int x,int y);//判断游戏是否结束
    void backStep(GPlayer *);//悔棋
    void giveup(GPlayer *);//认输
    void stop();//游戏进程停止，结束
signals:
    void gameoversignal(int,bool);//游戏结束信号（本地端）
    void gameonisok();//可以下棋信号
    void sendprogress(QString);//发送棋盘信息
    void unlock();//解除锁定信号
    void gameover();//游戏结束（联机）
    void stopt();//停止计时
    void startt();//开始计时
    void change(bool);//更改下棋方
};

#endif // GAMEMODEL_H


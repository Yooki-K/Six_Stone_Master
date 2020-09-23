#ifndef GAMEMODEL_H
#define GAMEMODEL_H
#include<vector>
#include<QMessageBox>
#include<QThread>
#include"database.hpp"
#include<QMainWindow>
#define PLAYER1 if(player1->aitype==none){\
QEventLoop loop;\
connect(c,SIGNAL(mouseRelease()),&loop,SLOT(quit()));\
loop.exec();\
}\
player1->myturn(c->clickx,c->clicky);


#define PLAYER2 if(player2->aitype==none){\
QEventLoop loop;\
connect(c,SIGNAL(mouseRelease()),&loop,SLOT(quit()));\
loop.exec();\
}\
player2->myturn(c->clickx,c->clicky);

#define SPLAYER1 QEventLoop loop;\
connect(c,SIGNAL(mouseRelease()),&loop,SLOT(quit()));\
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
connect(this,SIGNAL(sendprogress(QString)),this->parent(),SLOT(receiveprogress(QString)));\
emit sendprogress(progress);\
if(isstop) break;\
if(state!=playing){\
    QEventLoop loop;\
    QTimer::singleShot(1000,&loop,SLOT(quit()));\
    loop.exec();\
    emit gameover();\
    QEventLoop loopp;\
    connect(this,SIGNAL(unlock()),&loopp,SLOT(quit()));\
    loopp.exec();\
}

#define SPLAYER2 Gameflags=!Gameflags;\
QEventLoop looppp;\
connect(this->parent(),SIGNAL(isokon()),&looppp,SLOT(quit()));\
connect(this,SIGNAL(unlock()),&looppp,SLOT(quit()));\
looppp.exec();

class GPlayer;
class Chessboard;
class Gamemodel:public QThread
{
    Q_OBJECT
private:
    bool isstop=0;
public:
    GPlayer *player1=0;
    GPlayer *player2=0;
    Gametype type;
    Gamestate state;
    GameAI AItype;
    //必要数据
    Chessboard *c=0;
    int winx=0;
    int winy=0;
    int backx=-1;int backy=-1;
    bool Gameflags=1;//第一下棋方，黑棋
    std::vector<std::vector<what>> game_progress;
    int black_score[rowline][columnline];
    int white_score[rowline][columnline];
    int derect=-1;
    bool isfirst=1;
    int isonline=-1;//-1没联网，0创建棋盘，1不创建棋盘
    Gamemodel(QObject *parent=nullptr,int n=-1);
    ~Gamemodel();
    void run();
    int IsSix(int x,int y);
    Gamestate GameEnd(int x,int y);
    void backStep(GPlayer *);//悔棋
    void giveup(GPlayer *);//认输
    void stop();
signals:
    void gameoversignal(Gamestate,bool);
    void gameonisok();//可以下棋信号
    void sendprogress(QString);//发送棋盘信息
    void unlock();//解除锁定
    void gameover();
public slots:

};

#endif // GAMEMODEL_H


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
    int isonline=-1;
    Gamemodel(QObject *parent=nullptr);
    ~Gamemodel();
    void run();
    int IsSix(int x,int y);
    Gamestate GameEnd(int x,int y);
    void backStep(GPlayer *);//悔棋
    void giveup(GPlayer *);//认输
    void stop();

signals:
    void gameoversignal(int,bool);
    void gameonisok();//可以下棋信号
    void sendprogress(QString);//发送棋盘信息
    void unlock();//解除锁定
    void gameover();
    void stopt();
    void startt();
public slots:

};

#endif // GAMEMODEL_H


#ifndef GPLAYER_H
#define GPLAYER_H
#include"database.hpp"
#include <QObject>
#include<QTimer>
#include "gamemodel.h"
class Gamemodel;
struct maxscore//坐标及其得分情况
{
    int x=-1;int y=-1;//坐标
    int score=-1;//得分
    maxscore() {}
};
class GPlayer : public QObject
{
    Q_OBJECT
private:

public:
    bool myflag;//黑白方（真为黑  假为白）
    GameAI aitype=none;//机器人难度模式
    int backx=0;int backy=0;//最新落子坐标
    Gamemodel *game=0;//游戏进程指针
    QString name;//用户名
    bool istimeover=0;
    QString matchname;
    QTimer *ontime=0;//计时器 剩余时间
    ~GPlayer();
    explicit GPlayer(bool,Gamemodel *game=0, QObject *parent=0 ,  QString name="Player");
    GPlayer();
    //
    bool isRanged(int n);//判断是否超出棋盘范围
    void waitsec(int sec);//时间循环 等待函数
    maxscore maxblack();//返回黑棋得分最高的坐标，机器人在此落子
    maxscore maxwhite();//返回白棋得分最高的坐标，机器人在此落子
    void calculatBlack(int x,int y);//计算黑棋得分 简单模式
    void calculatWhite(int x,int y);//计算白棋得分 简单模式
    void calculatWhiteUp(int x,int y);//计算白棋得分 困难模式
    void calculatBlackUp(int fx, int fy);//计算黑棋得分 困难模式
    virtual void myturn( int x, int y);//虚函数 落子函数
    QString inttotime(int);//将时间从int转换为Qstring

signals:

public slots:

};

#endif // GPLAYER_H


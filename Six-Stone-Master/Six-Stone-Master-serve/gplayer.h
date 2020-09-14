#ifndef GPLAYER_H
#define GPLAYER_H

#include <QObject>
#include"gamemodel.h"
#include<QObject>
#include<QTimer>
#include<QEventLoop>
class Chessboard;
struct maxscore
{
    int x=-1;
    int y=-1;
    int score=-1;
    maxscore() {}
};
class GPlayer : public QObject
{
    Q_OBJECT
public:
    bool myflag;GameAI aitype=none;int backx=0;int backy=0;

    explicit GPlayer(QObject *parent = nullptr);
    GPlayer(bool flag, QObject *parent = nullptr);
    GPlayer();
    bool isRanged(int n);
    void waitsec(int sec);
    maxscore maxblack(Gamemodel *game);
    maxscore maxwhite(Gamemodel *game);
    void calculatBlack(Gamemodel *game,int x,int y);
    void calculatWhite(Gamemodel *game,int x,int y);
    void calculatWhiteUp(Gamemodel *game,int x,int y);
    void calculatBlackUp(Gamemodel *game,int fx, int fy);//算法为AI专有
    virtual void myturn(Gamemodel *game, int x, int y);


signals:
    void gameover(Gamestate,bool);
public slots:
    void dy();
};

#endif // GPLAYER_H


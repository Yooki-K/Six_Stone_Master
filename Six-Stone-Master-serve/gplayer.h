#ifndef GPLAYER_H
#define GPLAYER_H
#include"database.hpp"
#include <QObject>
#include<QTimer>
#include "gamemodel.h"
class Gamemodel;
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
private:

public:
    bool myflag;GameAI aitype=none;int backx=0;int backy=0;
    Gamemodel *game=0;QString name;QTimer *ontime=0;int pixindex=0;

    explicit GPlayer(bool,Gamemodel *game=0, QObject *parent=0 ,  QString name="Player");
    GPlayer();
    ~GPlayer();
    //
    bool isRanged(int n);
    void waitsec(int sec);
    maxscore maxblack();
    maxscore maxwhite();
    void calculatBlack(int x,int y);
    void calculatWhite(int x,int y);
    void calculatWhiteUp(int x,int y);
    void calculatBlackUp(int fx, int fy);//算法为AI专有
    virtual void myturn( int x, int y);
    QString inttotime(int);

signals:
    void gameover(int,bool);

public slots:

};

#endif // GPLAYER_H


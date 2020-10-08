#ifndef AI_H
#define AI_H
#include "gplayer.h"
#include<QtAlgorithms>
#include <QtGlobal>
#include<QTime>


class AI : public GPlayer
{
    Q_OBJECT
public:
    explicit AI(bool flag, QObject *parent = nullptr, Gamemodel *game=0, GameAI type=easy, QString name="AI");
    void myturn(int x, int y);//落子

};

#endif // AI_H

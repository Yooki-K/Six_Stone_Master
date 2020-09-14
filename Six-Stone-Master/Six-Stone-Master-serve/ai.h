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
    explicit AI(bool flag,QObject *parent = nullptr, GameAI type=easy);
    void myturn(Gamemodel *game, int x, int y);

};

#endif // AI_H

#ifndef GAMEMODEL_H
#define GAMEMODEL_H
#include<vector>
#include<QtAlgorithms>
enum Gametype{MM,MA,AA};////游戏模式
enum Gamestate{win,playing,death};//游戏状态
const int margin=45;
const int rowline=21;
const int columnline=21;
const int r=20;
const int one=45;
enum what{isempty=-1,iswhite,isblack};
class Gamemodel
{
private:


public:
    Gametype type;
    int winx=0;
    int winy=0;
    bool Gameflags=1;//下棋方
    bool isAA=0;
    std::vector<std::vector<what>> game_progress;
    int black_score[rowline][columnline];
    int white_score[rowline][columnline];
    Gamestate state;
    int derect=-1;
    Gamemodel();
    int IsSix(int x,int y);
    Gamestate GameEnd(int x,int y);
    void calculatBlack(int x,int y);
    void calculatWhite(int x,int y);
    bool isRanged(int n);
};

#endif // GAMEMODEL_H


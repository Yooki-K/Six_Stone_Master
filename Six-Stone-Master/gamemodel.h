#ifndef GAMEMODEL_H
#define GAMEMODEL_H
#include<vector>
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
    bool Gameflags=1;//下棋方
    std::vector<std::vector<what>> game_progress;
    std::vector<std::vector<int>> game_scores;
    Gamestate state;
    int derect=-1;
    Gamemodel();
    int IsSix(int x,int y);
    Gamestate GameEnd(int x,int y);
};

#endif // GAMEMODEL_H


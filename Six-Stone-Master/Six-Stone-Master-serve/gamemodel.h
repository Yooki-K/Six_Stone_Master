#ifndef GAMEMODEL_H
#define GAMEMODEL_H
#include<vector>
#include<QMessageBox>
enum Gametype{MM,MA,AA};//游戏模式,创建游戏时赋值
enum Gamestate{win,playing,death,waiting};//游戏状态,初始化时设置
enum GameAI{easy,diff,none};//AI模式,创建游戏时赋值
const int margin=45;
const int rowline=21;
const int columnline=21;
const int r=20;
const int one=45;
enum what{isempty=-1,iswhite,isblack};
class GPlayer;//声明玩家类;
class Gamemodel
{
    friend class Player;
private:
public:
    GPlayer *player1;
    GPlayer *player2;
    Gametype type;
    int winx=0;
    int winy=0;
    int backx=-1;int backy=-1;
    bool Gameflags=1;//第一下棋方，黑棋
    std::vector<std::vector<what>> game_progress;
    int black_score[rowline][columnline];
    int white_score[rowline][columnline];
    Gamestate state;
    int derect=-1;
    bool isfirst=1;
    GameAI AItype;
    Gamemodel();
    int IsSix(int x,int y);
    Gamestate GameEnd(int x,int y);
};

#endif // GAMEMODEL_H


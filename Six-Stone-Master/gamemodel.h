#ifndef GAMEMODEL_H
#define GAMEMODEL_H
enum mode{MM,MA,AA};////游戏模式
enum state{win,playing,death};//游戏状态
enum flags{white,black};//下棋方
class Gamemodel
{
private:


public:
    mode modeling;
    flags flagsing;
    Gamemodel();
};

#endif // GAMEMODEL_H

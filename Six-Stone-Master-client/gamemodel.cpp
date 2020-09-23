#include "gamemodel.h"
#include"chessboard.h"
#include"gplayer.h"
Gamemodel::Gamemodel(QObject *parent):QThread(parent){//parent为client
    state=playing;//设置游戏模式为进行中
    //棋盘初始化
    for(int i=0;i<columnline;i++)
    {
        game_progress.push_back(std::vector<what>());
    }
    for(int i=0;i<columnline;i++)//行
    {
        for(int j=0;j<rowline;j++)//添加21列
        {
            game_progress[i].push_back(isempty);
        }
    }
    //AI得分初始化
    for(int i=0;i<columnline;i++)
    {
        for(int j=0;j<rowline;j++)
        {
            black_score[i][j]=0;
        }
    }
    for(int i=0;i<columnline;i++)
    {
        for(int j=0;j<rowline;j++)
        {
            white_score[i][j]=0;
        }
    }
    connect(this,SIGNAL(gameover()),this->parent()->parent(),SLOT(GameOver()));
}

Gamemodel::~Gamemodel()
{
    if(c!=0)delete c;
    if(player1!=0)delete player1;
    if(player2!=0)delete player2;
}
void Gamemodel::run()
{
    while(!isstop){
        if(player1->myflag)
        {
            SPLAYER1
            c->Sound_effect->play();
            c->update();
            SPLAYER2
            c->Sound_effect->play();
            c->update();
        }
        else{
            SPLAYER2
            c->Sound_effect->play();
            c->update();
            SPLAYER1
            c->Sound_effect->play();
            c->update();

        }
    }
}
Gamestate Gamemodel::GameEnd(int x, int y)
{
    bool isdeath=1;
    for(int i=0;i<columnline;i++)
    {
        auto t=std::find(game_progress[i].begin(),game_progress[i].end(),isempty);
        if(t==game_progress[i].end()) {}
        else isdeath=0;
    }
    if(isdeath)return death;
    derect=IsSix(x,y);
    if(derect>=0) return win;
    else return playing;
}

int Gamemodel::IsSix(int x,int y)
{
    int num=1;
    for(int i=-5;i<=5;i++){
        if(y+i<0||y+i>20)continue;
        if(game_progress[x][y+i]==game_progress[x][y]) {num++; if(num==7) {winx=x;winy=y+i;return 0;}}
        else num=1;
    }
    num=1;
    for(int i=-5;i<=5;i++){
        if(y+i<0||y+i>20||x+i<0||x+i>20)continue;
        if(game_progress[x+i][y+i]==game_progress[x][y]) {num++; if(num==7) {winx=x+i;winy=y+i;return 1;}}
        else num=1;
    }
    num=1;
    for(int i=-5;i<=5;i++){
        if(x+i<0||x+i>20)continue;
        if(game_progress[x+i][y]==game_progress[x][y]) {num++;if(num==7){ winx=x+i;winy=y;return 2;}}
        else num=1;
    }
    num=1;
    for(int i=-5;i<=5;i++){
        if(y-i<0||y-i>20||x+i<0||x+i>20)continue;
        if(game_progress[x+i][y-i]==game_progress[x][y]) {num++;if(num==7) {winx=x+i;winy=y-i;return 3;}}
        else num=1;
    }
        return -1;

}

void Gamemodel::backStep(GPlayer *p)
{
    if(backx==p->backx&&backy==p->backy)
    {
        game_progress[p->backx][p->backy]=isempty;
    }
    else
    {
        game_progress[player2->backx][player2->backy]=isempty;
        game_progress[player1->backx][player1->backy]=isempty;
    }
//分数也要进行存储
//    不进行分数存储，人机模式默认AI拒绝；
//
 if(c!=0)
     c->update();
}

void Gamemodel::giveup(GPlayer *p)
{
//    state=win;
//    connect(this,SIGNAL(gameoversignal()),this->parent(),SLOT(GameOver()));
//    emit gameoversignal(state,!p->myflag);
}

void Gamemodel::stop()
{
    isstop=1;
    emit unlock();
    wait();
}






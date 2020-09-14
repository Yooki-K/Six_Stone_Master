#include "gamemodel.h"

Gamemodel::Gamemodel()
{
    state=playing;
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




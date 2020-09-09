#include "gamemodel.h"

Gamemodel::Gamemodel()
{
    state=playing;
    for(int i=0;i<columnline;i++)
    {
        game_progress.push_back(std::vector<what>());//不断加行
    }
    for(int i=0;i<columnline;i++)//行
    {
        for(int j=0;j<rowline;j++)//添加21列
        {
            game_progress[i].push_back(isempty);
        }
    }
}

Gamestate Gamemodel::GameEnd(int x, int y)
{
    derect=IsSix(x,y);
    if(derect>0)
        return win;
    else
        return playing;
}

int Gamemodel::IsSix(int x,int y)
{
        if(y>4){
            if(game_progress[x][y]==game_progress[x][y-1]&&game_progress[x][y]==game_progress[x][y-2]&&game_progress[x][y]==game_progress[x][y-3]&&game_progress[x][y]==game_progress[x][y-4]&&game_progress[x][y]==game_progress[x][y-5])
                return 0;
        }
        if(y>4&&x<margin-4){
            if(game_progress[x][y]==game_progress[x+1][y-1]&&game_progress[x][y]==game_progress[x+2][y-2]&&game_progress[x][y]==game_progress[x+3][y-3]&&game_progress[x][y]==game_progress[x+4][y-4]&&game_progress[x][y]==game_progress[x+5][y-5])
                return 1;
        }
        if(x<margin-4){
            if(game_progress[x][y]==game_progress[x+1][y]&&game_progress[x][y]==game_progress[x+2][y]&&game_progress[x][y]==game_progress[x+3][y]&&game_progress[x][y]==game_progress[x+4][y]&&game_progress[x][y]==game_progress[x+5][y])
                return 2;
        }
        if(x<margin-4&&y<margin-4){
            if(game_progress[x][y]==game_progress[x+1][y+1]&&game_progress[x][y]==game_progress[x+2][y+2]&&game_progress[x][y]==game_progress[x+3][y+3]&&game_progress[x][y]==game_progress[x+4][y+4]&&game_progress[x][y]==game_progress[x+5][y+5])
                return 3;
        }
        if(y<margin-4){
            if(game_progress[x][y]==game_progress[x][y+1]&&game_progress[x][y]==game_progress[x][y+2]&&game_progress[x][y]==game_progress[x][y+3]&&game_progress[x][y]==game_progress[x][y+4]&&game_progress[x][y]==game_progress[x][y+5])
               return 4;
        }
        if(y<margin-4&&x>4){
            if(game_progress[x][y]==game_progress[x-1][y+1]&&game_progress[x][y]==game_progress[x-2][y+2]&&game_progress[x][y]==game_progress[x-3][y+3]&&game_progress[x][y]==game_progress[x-4][y+4]&&game_progress[x][y]==game_progress[x-5][y+5])
                return 5;
        }
        if(x>4){
            if(game_progress[x][y]==game_progress[x-1][y]&&game_progress[x][y]==game_progress[x-2][y]&&game_progress[x][y]==game_progress[x-3][y]&&game_progress[x][y]==game_progress[x-4][y]&&game_progress[x][y]==game_progress[x-5][y])
                return 6;
        }
        if(x>4&&y>4){
            if(game_progress[x][y]==game_progress[x-1][y-1]&&game_progress[x][y]==game_progress[x-2][y-2]&&game_progress[x][y]==game_progress[x-3][y-3]&&game_progress[x][y]==game_progress[x-4][y-4]&&game_progress[x][y]==game_progress[x-5][y-5])
                return 7;
        }
        return -1;

}


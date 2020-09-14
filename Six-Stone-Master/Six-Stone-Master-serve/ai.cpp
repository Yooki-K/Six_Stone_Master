#include "ai.h"

AI::AI(bool flag, QObject *parent, GameAI type):GPlayer(flag,parent)
{
    aitype=type;
    connect(this,SIGNAL(gameover(Gamestate,bool)),this->parent()->parent(),SLOT(GameOver(Gamestate,bool)));
}

void AI::myturn(Gamemodel *game, int x, int y)
{
    x=0;y=0;//
    if(game->isfirst) {
        QTime time= QTime::currentTime();
        qsrand(time.msec()+time.second()*3000);  //设置随机种子
        int fx =qrand()%3+10;int fy =qrand()%3+10;
        if(game->game_progress[fx][fy]==isempty)
        game->game_progress[fx][fy]=(what)myflag;
        game->black_score[fx][fy]=-2000;
        game->white_score[fx][fy]=-2000;
        backx=fx;backy=fy;
        game->isfirst=0;
        if(myflag) {
            if(aitype==easy)     calculatBlack(game,fx,fy);
            if(aitype==diff)     calculatBlackUp(game,fx,fy);
        }
        else{
            if(aitype==easy)     calculatWhite(game,fx,fy);
            if(aitype==diff)     calculatWhiteUp(game,fx,fy);
        }//计算此子
        game->Gameflags=!game->Gameflags;//换手
        game->backx=backx;game->backy=backy;
    }
    else
    {
        waitsec(1000);
        maxscore maxBlack=maxblack(game);
        maxscore maxWhite=maxwhite(game);
        if(maxBlack.score>maxWhite.score){
            game->game_progress[maxBlack.x][maxBlack.y]=(what)myflag;
            game->state=game->GameEnd(maxBlack.x,maxBlack.y);
            if(game->state==win)
            {emit gameover(game->state,myflag);}
            game->black_score[maxBlack.x][maxBlack.y]=-2000;
            game->white_score[maxBlack.x][maxBlack.y]=-2000;
            backx=maxBlack.x;backy=maxBlack.y;
        }else{
            game->game_progress[maxWhite.x][maxWhite.y]=(what)myflag;
            game->state=game->GameEnd(maxWhite.x,maxWhite.y);
            if(game->state==win)
            {emit gameover(game->state,myflag);}
            game->black_score[maxWhite.x][maxWhite.y]=-2000;
            game->white_score[maxWhite.x][maxWhite.y]=-2000;
            backx=maxWhite.x;backy=maxWhite.y;
        }
        if(myflag) {
            if(aitype==easy)     calculatBlack(game,maxWhite.x,maxWhite.y);
            if(aitype==diff)     calculatBlackUp(game,maxWhite.x,maxWhite.y);
        }
        else{
            if(aitype==easy)     calculatWhite(game,maxWhite.x,maxWhite.y);
            if(aitype==diff)     calculatWhiteUp(game,maxWhite.x,maxWhite.y);
        }//计算此子
    game->Gameflags=!game->Gameflags;//换手
    game->backx=backx;game->backy=backy;
    }
}



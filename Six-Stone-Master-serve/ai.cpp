#include "ai.h"

AI::AI(bool flag,  QObject *parent, Gamemodel *game, GameAI type,QString name):GPlayer(flag,game,parent,name)
{
    aitype=type;
    connect(this,SIGNAL(gameover(int,bool)),this->parent()->parent(),SLOT(GameOver(int,bool)));
}

void AI::myturn(int x, int y)//机器人下棋函数
{
    if(game->isstop) return;
    if(game->isfirst) {//开始时在一定范围内随机落子
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
            if(aitype==easy)     calculatBlack(fx,fy);
            if(aitype==diff)     calculatBlackUp(fx,fy);
        }
        else{
            if(aitype==easy)     calculatWhite(fx,fy);
            if(aitype==diff)     calculatWhiteUp(fx,fy);
        }//计算此子
        game->Gameflags=!game->Gameflags;//换手
        game->backx=backx;game->backy=backy;
    }
    else
    {
        waitsec(700);//落子等待0.7秒
        maxscore maxBlack=maxblack();//得到黑方最高分坐标
        maxscore maxWhite=maxwhite();//得到白方最高分坐标
        //得到最高分坐标，落子，将此位置分数设为-2000
        if(maxBlack.score>maxWhite.score){
            game->game_progress[maxBlack.x][maxBlack.y]=(what)myflag;
            game->state=game->GameEnd(maxBlack.x,maxBlack.y);
            if(game->state!=playing)
            {emit gameover(game->state,myflag);game->isstop=1;return;}
            game->black_score[maxBlack.x][maxBlack.y]=-2000;
            game->white_score[maxBlack.x][maxBlack.y]=-2000;
            backx=maxBlack.x;backy=maxBlack.y;
            if(myflag) {
                if(aitype==easy)     calculatBlack(maxBlack.x,maxBlack.y);
                if(aitype==diff)     calculatBlackUp(maxBlack.x,maxBlack.y);
            }
            else{
                if(aitype==easy)     calculatWhite(maxBlack.x,maxBlack.y);
                if(aitype==diff)     calculatWhiteUp(maxBlack.x,maxBlack.y);
            }//计算此落子造成的分数变动
        }else{
            game->game_progress[maxWhite.x][maxWhite.y]=(what)myflag;
            game->state=game->GameEnd(maxWhite.x,maxWhite.y);
            if(game->state!=playing)
            {emit gameover(game->state,myflag);game->isstop=1;return;}
            game->black_score[maxWhite.x][maxWhite.y]=-2000;
            game->white_score[maxWhite.x][maxWhite.y]=-2000;
            backx=maxWhite.x;backy=maxWhite.y;
            if(myflag) {
                if(aitype==easy)     calculatBlack(maxWhite.x,maxWhite.y);
                if(aitype==diff)     calculatBlackUp(maxWhite.x,maxWhite.y);
            }
            else{
                if(aitype==easy)     calculatWhite(maxWhite.x,maxWhite.y);
                if(aitype==diff)     calculatWhiteUp(maxWhite.x,maxWhite.y);
            }//计算此落子造成的分数变动
        }
    game->Gameflags=!game->Gameflags;//换手
    game->backx=backx;game->backy=backy;//存储落子坐标
    }
}



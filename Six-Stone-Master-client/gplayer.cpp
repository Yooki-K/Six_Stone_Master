#include "gplayer.h"
#include<QDebug>



GPlayer::GPlayer(bool flag, Gamemodel *game, QObject *parent, QString name) : QObject(parent),game(game),name(name)
{
    myflag=flag;
    ontime=new QTimer(this);
    ontime->setInterval(1000*20*60);
    connect(ontime,&QTimer::timeout,this,[&](){
        this->game->state=win;
        istimeover=1;
        emit this->game->gameover();//计时器超过20分钟，游戏失败
    });
}

QString GPlayer::inttotime(int sum)//将时间从int转换为Qstring，并填充，格式为：__:__
{
    sum/=1000;
    int m=sum/60;
    int s=sum%60;
    return QString(QString("%1:%2").arg(m,2,10,QLatin1Char( '0' )).arg(s,2,10,QLatin1Char( '0' )));
}

GPlayer::GPlayer()
{
}
GPlayer::~GPlayer()
{
    game=0;
}

void GPlayer::myturn(int x,int y){//落子后更新棋盘信息，ai模式还需计算得分
    game->game_progress[x][y]=(what)myflag;
    if(game->state!=playing)
    {return;}
    game->state=game->GameEnd(x,y);
    if(game->state!=playing)
    {return;}
    game->black_score[x][y]=-2000;
    game->white_score[x][y]=-2000;
    if(game->type!=MM){
        if(myflag) {
            if(game->AItype==easy)     calculatBlack(x,y);
            if(game->AItype==diff)     calculatBlackUp(x,y);
        }
        else{
            if(game->AItype==easy)     calculatWhite(x,y);
            if(game->AItype==diff)     calculatWhiteUp(x,y);
    }//计算此子

 }
    game->Gameflags=!game->Gameflags;//换手
    backx=x;backy=y;
    game->backx=x;game->backy=y;//存储最新落子信息
}

void GPlayer::calculatBlack(int fx, int fy)//黑方计分算法（简单模式），白方同
{
    int i=1;
    while(1){//其中一方向代码，其他方向类似
        if(!isRanged(fy-i))break;
        if(game->game_progress[fx][fy-i]==isblack) {
            i++;
        }
        else
        {
            if(game->game_progress[fx][fy-i]==iswhite){
                if(i>=4&&fy+1<21)game->black_score[fx][fy+1]+=100;
                break;
            }
            if(game->game_progress[fx][fy-i]==isempty) {
                game->black_score[fx][fy-i]+=10*i;
                if(fy+1<21&&i!=1)
                    game->black_score[fx][fy+1]+=10*i;
                break;
            }
        }
    }
   i=1;
    while(1){
        if(!isRanged(fx+i)||!isRanged(fy-i)) break;
        if(game->game_progress[fx+i][fy-i]==isblack) {
            i++;
        }else
       {
        if(game->game_progress[fx+i][fy-i]==iswhite){
            if(fx-1>-1&&fy+1<21&&i>=4)game->black_score[fx-1][fy+1]+=100;
            break;
        }
        if(game->game_progress[fx+i][fy-i]==isempty) {
            game->black_score[fx+i][fy-i]+=10*i;
            if(fx-1>-1&&fy+1<21&&i!=1)
            game->black_score[fx-1][fy+1]+=10*i;
            break;
        }
       }
    }
i=1;
    while(1){

        if(!isRanged(fx+i)) break;
        if(game->game_progress[fx+i][fy]==isblack) {
            i++;
        }
        else
        {
            if(game->game_progress[fx+i][fy]==iswhite){
                if(fx-1>-1&&i>=4)game->black_score[fx-1][fy]+=100;
                break;
            }
            if(game->game_progress[fx+i][fy]==isempty) {
                game->black_score[fx+i][fy]+=10*i;
                if(fx-1>-1&&i!=1)
                    game->black_score[fx-1][fy]+=10*i;
                break;
            }
        }
    }
    i=1;
    while(1){
        if(!isRanged(fy+i)||!isRanged(fx+i))  break;
        if( game->game_progress[fx+i][fy+i]==isblack) {
            i++;
        }
        else
        {
            if( game->game_progress[fx+i][fy+i]==iswhite) {
                if(fx-1>-1&&fy-1>-1&&i>=4)game->black_score[fx-1][fy-1]+=100;
                break;
            }
            if(game->game_progress[fx+i][fy+i]==isempty) {
                game->black_score[fx+i][fy+i]+=10*i;
                if(fx-1>-1&&fy-1>-1&&i!=1)
                    game->black_score[fx-1][fy-1]+=10*i;
                break;
            }
        }

    }
    i=1;
    while(1){
        if(!isRanged(fy+i)) break;
        if(game->game_progress[fx][fy+i]==isblack) {
            i++;
        }
        else
        {
            if(game->game_progress[fx][fy+i]==iswhite) {
                if(fy-1>-1&&i>=4)game->black_score[fx][fy-1]+=100;
                break;
            }
            if(game->game_progress[fx][fy+i]==isempty) {
                game->black_score[fx][fy+i]+=10*i;
                if(fy-1>-1&&i!=1)
                    game->black_score[fx][fy-1]+=10*i;
                break;
            }
        }
    }
    i=1;
    while(1){
        if(!isRanged(fy+i)||!isRanged(fx-i))  break;
        if(game->game_progress[fx-i][fy+i]==isblack) {
            i++;
        }
        else
        {
            if(game->game_progress[fx-i][fy+i]==iswhite) {
                if(fx+1<21&&fy-1>-1&&i>=4)game->black_score[fx+1][fy-1]+=100;
                break;
            }
            if(game->game_progress[fx-i][fy+i]==isempty) {
                game->black_score[fx-i][fy+i]+=10*i;
                if(fx+1<21&&fy-1>-1&&i!=1)
                    game->black_score[fx+1][fy-1]+=10*i;
                break;
            }
        }

    }
    i=1;
    while(1){
        if(!isRanged(fx-i)) break;
        if(game->game_progress[fx-i][fy]==isblack) {
            i++;
        }
        else
        {
            if(game->game_progress[fx-i][fy]==iswhite) {
                if(fx+1<21&&i>=4)game->black_score[fx+1][fy]+=100;
                break;
            }
            if(game->game_progress[fx-i][fy]==isempty) {
                game->black_score[fx-i][fy]+=10*i;
                if(fx+1<21&&i!=1)
                    game->black_score[fx+1][fy]+=10*i;
                break;
            }
        }
    }
    i=1;
    while(1){
        if(!isRanged(fy-i)||!isRanged(fx-i))  break;
        if(game->game_progress[fx-i][fy-i]==isblack) {
            i++;
        }
        else
        {
            if(game->game_progress[fx-i][fy-i]==iswhite) {
                if(fx+1<21&&fy+1<21&&i>=4)game->black_score[fx+1][fy+1]+=100;
                break;
            }
            if(game->game_progress[fx-i][fy-i]==isempty) {
                game->black_score[fx-i][fy-i]+=10*i;
                if(fx+1<21&&fy+1<21&&i!=1)
                    game->black_score[fx+1][fy+1]+=10*i;
                break;
            }
        }
    }
}

void GPlayer::calculatWhite(int fx, int fy)
{
    int i=1;
    while(1){
        if(!isRanged(fy-i))  break;
        if(game->game_progress[fx][fy-i]==iswhite) {
            i++;
        }
        else
        {
            if(game->game_progress[fx][fy-i]==isblack){
                if(fy+1<21&&i>=4)game->white_score[fx][fy+1]+=100;
                break;
            }
            if(game->game_progress[fx][fy-i]==isempty) {
                game->white_score[fx][fy-i]+=10*i;
                if(fy+1<21&&i!=1)
                    game->white_score[fx][fy+1]+=10*i;
                break;
            }
        }
    }
   i=1;
    while(1){
        if(!isRanged(fx+i)||!isRanged(fy-i))  break;
        if(game->game_progress[fx+i][fy-i]==iswhite) {
            i++;
        }
        else
        {
            if(game->game_progress[fx+i][fy-i]==isblack){
                if(fx-1>-1&&fy+1<21&&i>=4)game->white_score[fx-1][fy+1]+=100;
                break;
            }
            if(game->game_progress[fx+i][fy-i]==isempty) {
                game->white_score[fx+i][fy-i]+=10*i;
                if(fx-1>-1&&fy+1<21&&i!=1)
                    game->white_score[fx-1][fy+1]+=10*i;
                break;
            }
        }

    }
i=1;
    while(1){

        if(!isRanged(fx+i))  break;
        if(game->game_progress[fx+i][fy]==iswhite) {
            i++;
        }
        else
        {
            if(game->game_progress[fx+i][fy]==isblack){
                if(fx-1>-1&&i>=4)game->white_score[fx-1][fy]+=100;
                break;
            }
            if(game->game_progress[fx+i][fy]==isempty) {
                game->white_score[fx+i][fy]+=10*i;
                if(fx-1>-1&&i!=1)
                    game->white_score[fx-1][fy]+=10*i;
                break;
            }
        }
    }
    i=1;
    while(1){
        if(!isRanged(fy+i)||!isRanged(fx+i))  break;
        if( game->game_progress[fx+i][fy+i]==iswhite) {
            i++;
        }
        else
        {
            if( game->game_progress[fx+i][fy+i]==isblack) {
                if(fx-1>-1&&fy-1>-1&&i>=4)game->white_score[fx-1][fy-1]+=100;
                break;
            }
            if(game->game_progress[fx+i][fy+i]==isempty) {
                game->white_score[fx+i][fy+i]+=10*i;
                if(fx-1>-1&&fy-1>-1&&i!=1)
                    game->white_score[fx-1][fy-1]+=10*i;
                break;
            }
        }

    }
    i=1;
    while(1){
        if(!isRanged(fy+i)) break;
        if(game->game_progress[fx][fy+i]==iswhite) {
            i++;
        }
        else
        {
            if(game->game_progress[fx][fy+i]==isblack) {
                if(fy-1>-1&&i>=4)game->white_score[fx][fy-1]+=100;
                break;
            }
            if(game->game_progress[fx][fy+i]==isempty) {
                game->white_score[fx][fy+i]+=10*i;
                if(fy-1>-1&&i!=1)
                    game->white_score[fx][fy-1]+=10*i;
                break;
            }
        }
    }
    i=1;
    while(1){
        if(!isRanged(fy+i)||!isRanged(fx-i))  break;
        if(game->game_progress[fx-i][fy+i]==iswhite) {
            i++;
        }
        else
        {
            if(game->game_progress[fx-i][fy+i]==isblack) {
                if(fx+1<21&&fy-1>-1&&i>=4)game->white_score[fx+1][fy-1]+=100;
                break;
            }
            if(game->game_progress[fx-i][fy+i]==isempty) {
                game->white_score[fx-i][fy+i]+=10*i;
                if(fx+1<21&&fy-1>-1&&i!=1)
                    game->white_score[fx+1][fy-1]+=10*i;
                break;
            }
        }

    }
    i=1;
    while(1){
        if(!isRanged(fx-i)) break;
        if(game->game_progress[fx-i][fy]==iswhite) {
            i++;
        }
        else
        {
            if(game->game_progress[fx-i][fy]==isblack) {
                if(fx+1<21&&i>=4)game->white_score[fx+1][fy]+=100;
                break;
            }
            if(game->game_progress[fx-i][fy]==isempty) {
                game->white_score[fx-i][fy]+=10*i;
                if(fx+1<21&&i!=1)
                    game->white_score[fx+1][fy]+=10*i;
                break;
            }
        }
    }
    i=1;
    while(1){
        if(!isRanged(fy-i)||!isRanged(fx-i))  break;
        if(game->game_progress[fx-i][fy-i]==iswhite) {
            i++;
        }
        else
        {
            if(game->game_progress[fx-i][fy-i]==isblack) {
                if(fx+1<21&&fy+1<21&&i>=4)game->white_score[fx+1][fy+1]+=100;
                break;
            }
            if(game->game_progress[fx-i][fy-i]==isempty) {
                game->white_score[fx-i][fy-i]+=10*i;
                if(fx+1<21&&fy+1<21&&i!=1)
                    game->white_score[fx+1][fy+1]+=10*i;
                break;
            }
        }
    }
}

bool GPlayer::isRanged(int n)
{
    if(n>=0&&n<columnline) return 1;
    else return 0;
}
void GPlayer::calculatWhiteUp(int fx, int fy)
{
    struct after
    {
        bool notfirst=0;
        what is=isempty;
        int n=0;
        after() {}
    }a;
    int i=1;a.n=0;a.notfirst=0;a.is=isempty;
    while(1){
        if(!isRanged(fy-i))  break;
        if(game->game_progress[fx][fy-i]==iswhite) {
            if(a.notfirst){
                if(a.is==iswhite)  a.n++;
                if(a.is==isempty)  {a.is=iswhite;a.n++;}
            }
            i++;
        }
        else
        {
            if(game->game_progress[fx][fy-i]==isblack){
                if(!a.notfirst){
                    if(fy+1<21&&i>=4) game->white_score[fx][fy+1]+=200;//直接连6
                    break;
                }//无空子
                a.n++;
                if(a.is==iswhite){
                    if(i>5) {game->white_score[fx][fy-i+a.n-1]+=200;break;}//直接连6
                    game->white_score[fx][fy-i+a.n-1]+=11*(i-1);break;
                }
                if(a.is==isempty){
                   if(i>5) {game->white_score[fx][fy-i+a.n-1]+=200;break;}//直接连6
                   if(fy+1<21&&i!=2)
                       game->white_score[fx][fy+1]+=5*(i-1);
                   game->white_score[fx][fy-i+a.n-1]+=10*(i-1);
                   break;
                }
            }
            if(game->game_progress[fx][fy-i]==isempty) {
                a.n++;
                if(a.notfirst){
                        game->white_score[fx][fy-i+a.n-1]+=15*(i-1);
                        if(a.n>2) game->white_score[fx][fy-i]+=10*(i-1);
                        if(fy+1<21&&i!=2)
                            game->white_score[fx][fy+1]+=10*(i-1);
                        break;
                }
                else{
                     a.notfirst=1;i++;
                }
            }
        }
    }
   i=1;a.n=0;a.notfirst=0;a.is=isempty;
   while(1){
       if(!isRanged(fx+i)||!isRanged(fy-i))  break;
       if(game->game_progress[fx+i][fy-i]==iswhite) {
           if(a.notfirst){
               if(a.is==iswhite)  a.n++;
               if(a.is==isempty)  {a.is=iswhite;a.n++;}
           }
           i++;
       }
       else
       {
           if(game->game_progress[fx+i][fy-i]==isblack){
               if(!a.notfirst){
                   if(fx-1>-1&&fy+1<21&&i>=4) game->white_score[fx-1][fy+1]+=200;//直接连6
                   break;
               }//无空子
               a.n++;
               if(a.is==iswhite){
                   if(i>5) {game->white_score[fx+i-a.n+1][fy-i+a.n-1]+=200;break;}//直接连6
                   game->white_score[fx+i-a.n+1][fy-i+a.n-1]+=11*(i-1);break;
               }
               if(a.is==isempty){
                  if(i>5) {game->white_score[fx+i-a.n+1][fy-i+a.n-1]+=200;break;}//直接连6
                  if(fx-1>-1&&fy+1<21&&i!=2)
                      game->white_score[fx-1][fy+1]+=5*(i-1);
                  game->white_score[fx+i-a.n+1][fy-i+a.n-1]+=10*(i-1);
                  break;
               }
           }
           if(game->game_progress[fx+i][fy-i]==isempty) {
               a.n++;
               if(a.notfirst){
                       game->white_score[fx+i-a.n+1][fy-i+a.n-1]+=15*(i-1);
                       if(a.n>2) game->white_score[fx+i][fy-i]+=10*(i-1);
                       if(fx-1>-1&&fy+1<21&&i!=2)
                           game->white_score[fx-1][fy+1]+=10*(i-1);
                       break;
               }
               else{
                    a.notfirst=1;i++;
               }
           }
       }
   }

   i=1;a.n=0;a.notfirst=0;a.is=isempty;
   while(1){
       if(!isRanged(fx+i))  break;
       if(game->game_progress[fx+i][fy]==iswhite) {
           if(a.notfirst){
               if(a.is==iswhite)  a.n++;
               if(a.is==isempty)  {a.is=iswhite;a.n++;}
           }
           i++;
       }
       else
       {
           if(game->game_progress[fx+i][fy]==isblack){
               if(!a.notfirst){
                   if(fx-1>-1&&i>=4) game->white_score[fx+i][fy]+=200;//直接连6
                   break;
               }//无空子
               a.n++;
               if(a.is==iswhite){
                   if(i>5) {game->white_score[fx+i-a.n+1][fy]+=200;break;}//直接连6
                   game->white_score[fx+i-a.n+1][fy]+=11*(i-1);break;
               }
               if(a.is==isempty){
                  if(i>5) {game->white_score[fx+i-a.n+1][fy]+=200;break;}//直接连6
                  if(fx-1>-1&&i!=2)
                      game->white_score[fx-1][fy]+=5*(i-1);
                  game->white_score[fx+i-a.n+1][fy]+=10*(i-1);
                  break;
               }
           }
           if(game->game_progress[fx+i][fy]==isempty) {
               a.n++;
               if(a.notfirst){
                       game->white_score[fx+i-a.n+1][fy]+=15*(i-1);
                       if(a.n>2) game->white_score[fx+i][fy]+=10*(i-1);
                       if(fx-1>-1&&i!=2)
                           game->white_score[fx-1][fy]+=10*(i-1);
                       break;
               }
               else{
                    a.notfirst=1;i++;
               }
           }
       }
   }

    i=1;a.n=0;a.notfirst=0;a.is=isempty;
    while(1){
        if(!isRanged(fx+i)||!isRanged(fy+i))  break;
        if(game->game_progress[fx+i][fy+i]==iswhite) {
            if(a.notfirst){
                if(a.is==iswhite)  a.n++;
                if(a.is==isempty)  {a.is=iswhite;a.n++;}
            }
            i++;
        }
        else
        {
            if(game->game_progress[fx+i][fy+i]==isblack){
                if(!a.notfirst){
                    if(fx-1>-1&&fy-1>-1&&i>=4) game->white_score[fx-1][fy-1]+=200;//直接连6
                    break;
                }//无空子
                a.n++;
                if(a.is==iswhite){
                    if(i>5) {game->white_score[fx+i-a.n+1][fy+i-a.n+1]+=200;break;}//直接连6
                    game->white_score[fx+i-a.n+1][fy+i-a.n+1]+=11*(i-1);break;
                }
                if(a.is==isempty){
                   if(i>5) {game->white_score[fx+i-a.n+1][fy+i-a.n+1]+=200;break;}//直接连6
                   if(fx-1>-1&&fy-1>-1&&i!=2)
                       game->white_score[fx-1][fy-1]+=5*(i-1);
                   game->white_score[fx+i-a.n+1][fy+i-a.n+1]+=10*(i-1);
                   break;
                }
            }
            if(game->game_progress[fx+i][fy+i]==isempty) {
                a.n++;
                if(a.notfirst){
                        game->white_score[fx+i-a.n+1][fy+i-a.n+1]+=15*(i-1);
                        if(a.n>2) game->white_score[fx+i][fy+i]+=10*(i-1);
                        if(fx-1>-1&&fy-1>-1&&i!=2)
                            game->white_score[fx-1][fy-1]+=10*(i-1);
                        break;
                }
                else{
                     a.notfirst=1;i++;
                }
            }
        }
    }

    i=1;a.n=0;a.notfirst=0;a.is=isempty;
    while(1){
        if(!isRanged(fy+i))  break;
        if(game->game_progress[fx][fy+i]==iswhite) {
            if(a.notfirst){
                if(a.is==iswhite)  a.n++;
                if(a.is==isempty)  {a.is=iswhite;a.n++;}
            }
            i++;
        }
        else
        {
            if(game->game_progress[fx][fy+i]==isblack){
                if(!a.notfirst){
                    if(fy-1>-1&&i>=4) game->white_score[fx][fy-1]+=200;//直接连6
                    break;
                }//无空子
                a.n++;
                if(a.is==iswhite){
                    if(i>5) {game->white_score[fx][fy+i-a.n+1]+=200;break;}//直接连6
                    game->white_score[fx][fy+i-a.n+1]+=11*(i-1);break;
                }
                if(a.is==isempty){
                   if(i>5) {game->white_score[fx][fy+i-a.n+1]+=200;break;}//直接连6
                   if(fy-1>-1&&i!=2)
                       game->white_score[fx][fy-1]+=5*(i-1);
                   game->white_score[fx][fy+i-a.n+1]+=10*(i-1);
                   break;
                }
            }
            if(game->game_progress[fx][fy+i]==isempty) {
                a.n++;
                if(a.notfirst){
                        game->white_score[fx][fy+i-a.n+1]+=15*(i-1);
                        if(a.n>2) game->white_score[fx][fy+i]+=10*(i-1);
                        if(fy-1>-1&&i!=2)
                            game->white_score[fx][fy-1]+=10*(i-1);
                        break;
                }
                else{
                     a.notfirst=1;i++;
                }
            }
        }
    }

    i=1;a.n=0;a.notfirst=0;a.is=isempty;
    while(1){
        if(!isRanged(fx-i)||!isRanged(fy+i))  break;
        if(game->game_progress[fx-i][fy+i]==iswhite) {
            if(a.notfirst){
                if(a.is==iswhite)  a.n++;
                if(a.is==isempty)  {a.is=iswhite;a.n++;}
            }
            i++;
        }
        else
        {
            if(game->game_progress[fx-i][fy+i]==isblack){
                if(!a.notfirst){
                    if(fx+1<21&&fy-1>-1&&i>=4) game->white_score[fx+1][fy-1]+=200;//直接连6
                    break;
                }//无空子
                a.n++;
                if(a.is==iswhite){
                    if(i>5) {game->white_score[fx-i+a.n-1][fy+i-a.n+1]+=200;break;}//直接连6
                    game->white_score[fx-i+a.n-1][fy+i-a.n+1]+=11*(i-1);break;
                }
                if(a.is==isempty){
                   if(i>5) {game->white_score[fx-i+a.n-1][fy+i-a.n+1]+=200;break;}//直接连6
                   if(fx+1<21&&fy-1>-1&&i!=2)
                       game->white_score[fx+1][fy-1]+=5*(i-1);
                   game->white_score[fx-i+a.n-1][fy+i-a.n+1]+=10*(i-1);
                   break;
                }
            }
            if(game->game_progress[fx-i][fy+i]==isempty) {
                a.n++;
                if(a.notfirst){
                        game->white_score[fx-i+a.n-1][fy+i-a.n+1]+=15*(i-1);
                        if(a.n>2) game->white_score[fx-i][fy+i]+=10*(i-1);
                        if(fx+1<21&&fy-1>-1&&i!=2)
                            game->white_score[fx+1][fy-1]+=10*(i-1);
                        break;
                }
                else{
                     a.notfirst=1;i++;
                }
            }
        }
    }

    i=1;a.n=0;a.notfirst=0;a.is=isempty;
    while(1){
        if(!isRanged(fx-i))  break;
        if(game->game_progress[fx-i][fy]==iswhite) {
            if(a.notfirst){
                if(a.is==iswhite)  a.n++;
                if(a.is==isempty)  {a.is=iswhite;a.n++;}
            }
            i++;
        }
        else
        {
            if(game->game_progress[fx-i][fy]==isblack){
                if(!a.notfirst){
                    if(fx+1<21&&i>=4) game->white_score[fx+1][fy]+=200;//直接连6
                    break;
                }//无空子
                a.n++;
                if(a.is==iswhite){
                    if(i>5) {game->white_score[fx-i+a.n-1][fy]+=200;break;}//直接连6
                    game->white_score[fx-i+a.n-1][fy]+=11*(i-1);break;
                }
                if(a.is==isempty){
                   if(i>5) {game->white_score[fx-i+a.n-1][fy]+=200;break;}//直接连6
                   if(fx+1<21&&i!=2)
                       game->white_score[fx+1][fy]+=5*(i-1);
                   game->white_score[fx-i+a.n-1][fy]+=10*(i-1);
                   break;
                }
            }
            if(game->game_progress[fx-i][fy]==isempty) {
                a.n++;
                if(a.notfirst){
                        game->white_score[fx-i+a.n-1][fy]+=15*(i-1);
                        if(a.n>2) game->white_score[fx-i][fy]+=10*(i-1);
                        if(fx+1<21&&i!=2)
                            game->white_score[fx+1][fy]+=10*(i-1);
                        break;
                }
                else{
                     a.notfirst=1;i++;
                }
            }
        }
    }

    i=1;a.n=0;a.notfirst=0;a.is=isempty;
    while(1){
        if(!isRanged(fx-i)||!isRanged(fy-i))  break;
        if(game->game_progress[fx-i][fy-i]==iswhite) {
            if(a.notfirst){
                if(a.is==iswhite)  a.n++;
                if(a.is==isempty)  {a.is=iswhite;a.n++;}
            }
            i++;
        }
        else
        {
            if(game->game_progress[fx-i][fy-i]==isblack){
                if(!a.notfirst){
                    if(fx+1<21&&fy+1<21&&i>=4) game->white_score[fx+1][fy+1]+=200;//直接连6
                    break;
                }//无空子
                a.n++;
                if(a.is==iswhite){
                    if(i>5) {game->white_score[fx-i+a.n-1][fy-i+a.n-1]+=200;break;}//直接连6
                    game->white_score[fx-i+a.n-1][fy-i+a.n-1]+=11*(i-1);break;
                }
                if(a.is==isempty){
                   if(i>5) {game->white_score[fx-i+a.n-1][fy-i+a.n-1]+=200;break;}//直接连6
                   if(fx+1<21&&fy+1<21&&i!=2)
                       game->white_score[fx+1][fy+1]+=5*(i-1);
                   game->white_score[fx-i+a.n-1][fy-i+a.n-1]+=10*(i-1);
                   break;
                }
            }
            if(game->game_progress[fx-i][fy-i]==isempty) {
                a.n++;
                if(a.notfirst){
                        game->white_score[fx-i+a.n-1][fy-i+a.n-1]+=15*(i-1);
                        if(a.n>2) game->white_score[fx-i][fy-i]+=10*(i-1);
                        if(fx+1<21&&fy+1<21&&i!=2)
                            game->white_score[fx+1][fy+1]+=10*(i-1);
                        break;
                }
                else{
                     a.notfirst=1;i++;
                }
            }
        }
    }

}

void GPlayer::calculatBlackUp(int fx, int fy)//黑方计分算法（困难模式），白方同
{

    struct after//下一位置情况
    {
        bool notfirst=0;//是否是落子周围最近一圈的棋子
        what is=isempty;//位置情况
        int n=0;//连续同一种棋子个数
        after() {}
    }a;
    int i=1;a.n=0;a.notfirst=0;a.is=isempty;
    while(1){
        if(!isRanged(fy-i))  break;
        if(game->game_progress[fx][fy-i]==isblack) {
            if(a.notfirst){
                if(a.is==isblack)  a.n++;
                if(a.is==isempty)  {a.is=isblack;a.n++;}
            }
            i++;
        }
        else
        {
            if(game->game_progress[fx][fy-i]==iswhite){
                if(!a.notfirst){
                    if(fy+1<21&&i>=4) game->black_score[fx][fy+1]+=200;//直接连6
                    break;
                }//无空子
                a.n++;
                if(a.is==isblack){
                    if(i>5) {game->black_score[fx][fy-i+a.n-1]+=200;break;}//直接连6
                    game->black_score[fx][fy-i+a.n-1]+=11*(i-1);break;
                }
                if(a.is==isempty){
                   if(i>5) {game->black_score[fx][fy-i+a.n-1]+=200;break;}//直接连6
                   if(fy+1<21&&i!=2)
                       game->black_score[fx][fy+1]+=5*(i-1);
                   game->black_score[fx][fy-i+a.n-1]+=10*(i-1);
                   break;
                }
            }
            if(game->game_progress[fx][fy-i]==isempty) {
                a.n++;
                if(a.notfirst){
                        game->black_score[fx][fy-i+a.n-1]+=15*(i-1);
                        if(a.n>2) game->black_score[fx][fy-i]+=10*(i-1);
                        if(fy+1<21&&i!=2)
                            game->black_score[fx][fy+1]+=10*(i-1);
                        break;
                }
                else{
                     a.notfirst=1;i++;
                }
            }
        }
    }
    i=1;a.n=0;a.notfirst=0;a.is=isempty;
    while(1){
       if(!isRanged(fx+i)||!isRanged(fy-i))  break;
       if(game->game_progress[fx+i][fy-i]==isblack) {
           if(a.notfirst){
               if(a.is==isblack)  a.n++;
               if(a.is==isempty)  {a.is=isblack;a.n++;}
           }
           i++;
       }
       else
       {
           if(game->game_progress[fx+i][fy-i]==iswhite){
               if(!a.notfirst){
                   if(fx-1>-1&&fy+1<21&&i>=4) game->black_score[fx-1][fy+1]+=200;//直接连6
                   break;
               }//无空子
               a.n++;
               if(a.is==isblack){
                   if(i>5) {game->black_score[fx+i-a.n+1][fy-i+a.n-1]+=200;break;}//直接连6
                   game->black_score[fx+i-a.n+1][fy-i+a.n-1]+=11*(i-1);break;
               }
               if(a.is==isempty){
                  if(i>5) {game->black_score[fx+i-a.n+1][fy-i+a.n-1]+=200;break;}//直接连6
                  if(fx-1>-1&&fy+1<21&&i!=2)
                      game->black_score[fx-1][fy+1]+=5*(i-1);
                  game->black_score[fx+i-a.n+1][fy-i+a.n-1]+=10*(i-1);
                  break;
               }
           }
           if(game->game_progress[fx+i][fy-i]==isempty) {
               a.n++;
               if(a.notfirst){
                       game->black_score[fx+i-a.n+1][fy-i+a.n-1]+=15*(i-1);
                       if(a.n>2) game->black_score[fx+i][fy-i]+=10*(i-1);
                       if(fx-1>-1&&fy+1<21&&i!=2)
                           game->black_score[fx-1][fy+1]+=10*(i-1);
                       break;
               }
               else{
                    a.notfirst=1;i++;
               }
           }
       }
    }

    i=1;a.n=0;a.notfirst=0;a.is=isempty;
    while(1){
       if(!isRanged(fx+i))  break;
       if(game->game_progress[fx+i][fy]==isblack) {
           if(a.notfirst){
               if(a.is==isblack)  a.n++;
               if(a.is==isempty)  {a.is=isblack;a.n++;}
           }
           i++;
       }
       else
       {
           if(game->game_progress[fx+i][fy]==iswhite){
               if(!a.notfirst){
                   if(fx-1>-1&&i>=4) game->black_score[fx-1][fy]+=200;//直接连6
                   break;
               }//无空子
               a.n++;
               if(a.is==isblack){
                   if(i>5) {game->black_score[fx+i-a.n+1][fy]+=200;break;}//直接连6
                   game->black_score[fx+i-a.n+1][fy]+=11*(i-1);break;
               }
               if(a.is==isempty){
                  if(i>5) {game->black_score[fx+i-a.n+1][fy]+=200;break;}//直接连6
                  if(fx-1>-1&&i!=2)
                      game->black_score[fx-1][fy]+=5*(i-1);
                  game->black_score[fx+i-a.n+1][fy]+=10*(i-1);
                  break;
               }
           }
           if(game->game_progress[fx+i][fy]==isempty) {
               a.n++;
               if(a.notfirst){
                       game->black_score[fx+i-a.n+1][fy]+=15*(i-1);
                       if(a.n>2) game->black_score[fx+i][fy]+=10*(i-1);
                       if(fx-1>-1&&i!=2)
                           game->black_score[fx-1][fy]+=10*(i-1);
                       break;
               }
               else{
                    a.notfirst=1;i++;
               }
           }
       }
    }

    i=1;a.n=0;a.notfirst=0;a.is=isempty;
    while(1){
        if(!isRanged(fx+i)||!isRanged(fy+i))  break;
        if(game->game_progress[fx+i][fy+i]==isblack) {
            if(a.notfirst){
                if(a.is==isblack)  a.n++;
                if(a.is==isempty)  {a.is=isblack;a.n++;}
            }
            i++;
        }
        else
        {
            if(game->game_progress[fx+i][fy+i]==iswhite){
                if(!a.notfirst){
                    if(fx-1>-1&&fy-1>-1&&i>=4) game->black_score[fx-1][fy-1]+=200;//直接连6
                    break;
                }//无空子
                a.n++;
                if(a.is==isblack){
                    if(i>5) {game->black_score[fx+i-a.n+1][fy+i-a.n+1]+=200;break;}//直接连6
                    game->black_score[fx+i-a.n+1][fy+i-a.n+1]+=11*(i-1);break;
                }
                if(a.is==isempty){
                   if(i>5) {game->black_score[fx+i-a.n+1][fy+i-a.n+1]+=200;break;}//直接连6
                   if(fx-1>-1&&fy-1>-1&&i!=2)
                       game->black_score[fx-1][fy-1]+=5*(i-1);
                   game->black_score[fx+i-a.n+1][fy+i-a.n+1]+=10*(i-1);
                   break;
                }
            }
            if(game->game_progress[fx+i][fy+i]==isempty) {
                a.n++;
                if(a.notfirst){
                        game->black_score[fx+i-a.n+1][fy+i-a.n+1]+=15*(i-1);
                        if(a.n>2) game->black_score[fx+i][fy+i]+=10*(i-1);
                        if(fx-1>-1&&fy-1>-1&&i!=2)
                            game->black_score[fx-1][fy-1]+=10*(i-1);
                        break;
                }
                else{
                     a.notfirst=1;i++;
                }
            }
        }
    }

    i=1;a.n=0;a.notfirst=0;a.is=isempty;
    while(1){
        if(!isRanged(fy+i))  break;
        if(game->game_progress[fx][fy+i]==isblack) {
            if(a.notfirst){
                if(a.is==isblack)  a.n++;
                if(a.is==isempty)  {a.is=isblack;a.n++;}
            }
            i++;
        }
        else
        {
            if(game->game_progress[fx][fy+i]==iswhite){
                if(!a.notfirst){
                    if(fy-1>-1&&i>=4) game->black_score[fx][fy-1]+=200;//直接连6
                    break;
                }//无空子
                a.n++;
                if(a.is==isblack){
                    if(i>5) {game->black_score[fx][fy+i-a.n+1]+=200;break;}//直接连6
                    game->black_score[fx][fy+i-a.n+1]+=11*(i-1);break;
                }
                if(a.is==isempty){
                   if(i>5) {game->black_score[fx][fy+i-a.n+1]+=200;break;}//直接连6
                   if(fy-1>-1&&i!=2)
                       game->black_score[fx][fy-1]+=5*(i-1);
                   game->black_score[fx][fy+i-a.n+1]+=10*(i-1);
                   break;
                }
            }
            if(game->game_progress[fx][fy+i]==isempty) {
                a.n++;
                if(a.notfirst){
                        game->black_score[fx][fy+i-a.n+1]+=15*(i-1);
                        if(a.n>2) game->black_score[fx][fy+i]+=10*(i-1);
                        if(fy-1>-1&&i!=2)
                            game->black_score[fx][fy-1]+=10*(i-1);
                        break;
                }
                else{
                     a.notfirst=1;i++;
                }
            }
        }
    }

    i=1;a.n=0;a.notfirst=0;a.is=isempty;
    while(1){
        if(!isRanged(fx-i)||!isRanged(fy+i))  break;
        if(game->game_progress[fx-i][fy+i]==isblack) {
            if(a.notfirst){
                if(a.is==isblack)  a.n++;
                if(a.is==isempty)  {a.is=isblack;a.n++;}
            }
            i++;
        }
        else
        {
            if(game->game_progress[fx-i][fy+i]==iswhite){
                if(!a.notfirst){
                    if(fx+1<21&&fy-1>-1&&i>=4) game->black_score[fx+1][fy-1]+=200;//直接连6
                    break;
                }//无空子
                a.n++;
                if(a.is==isblack){
                    if(i>5) {game->black_score[fx-i+a.n-1][fy+i-a.n+1]+=200;break;}//直接连6
                    game->black_score[fx-i+a.n-1][fy+i-a.n+1]+=11*(i-1);break;
                }
                if(a.is==isempty){
                   if(i>5) {game->black_score[fx-i+a.n-1][fy+i-a.n+1]+=200;break;}//直接连6
                   if(fx+1<21&&fy-1>-1&&i!=2)
                       game->black_score[fx+1][fy-1]+=5*(i-1);
                   game->black_score[fx-i+a.n-1][fy+i-a.n+1]+=10*(i-1);
                   break;
                }
            }
            if(game->game_progress[fx-i][fy+i]==isempty) {
                a.n++;
                if(a.notfirst){
                        game->black_score[fx-i+a.n-1][fy+i-a.n+1]+=15*(i-1);
                        if(a.n>2) game->black_score[fx-i][fy+i]+=10*(i-1);
                        if(fx+1<21&&fy-1>-1&&i!=2)
                            game->black_score[fx+1][fy-1]+=10*(i-1);
                        break;
                }
                else{
                     a.notfirst=1;i++;
                }
            }
        }
    }

    i=1;a.n=0;a.notfirst=0;a.is=isempty;
    while(1){
        if(!isRanged(fx-i))  break;
        if(game->game_progress[fx-i][fy]==isblack) {
            if(a.notfirst){
                if(a.is==isblack)  a.n++;
                if(a.is==isempty)  {a.is=isblack;a.n++;}
            }
            i++;
        }
        else
        {
            if(game->game_progress[fx-i][fy]==iswhite){
                if(!a.notfirst){
                    if(fx+1<21&&i>=4) game->black_score[fx+1][fy]+=200;//直接连6
                    break;
                }//无空子
                a.n++;
                if(a.is==isblack){
                    if(i>5) {game->black_score[fx-i+a.n-1][fy]+=200;break;}//直接连6
                    game->black_score[fx-i+a.n-1][fy]+=11*(i-1);break;
                }
                if(a.is==isempty){
                   if(i>5) {game->black_score[fx-i+a.n-1][fy]+=200;break;}//直接连6
                   if(fx+1<21&&i!=2)
                       game->black_score[fx+1][fy]+=5*(i-1);
                   game->black_score[fx-i+a.n-1][fy]+=10*(i-1);
                   break;
                }
            }
            if(game->game_progress[fx-i][fy]==isempty) {
                a.n++;
                if(a.notfirst){
                        game->black_score[fx-i+a.n-1][fy]+=15*(i-1);
                        if(a.n>2) game->black_score[fx-i][fy]+=10*(i-1);
                        if(fx+1<21&&i!=2)
                            game->black_score[fx+1][fy]+=10*(i-1);
                        break;
                }
                else{
                     a.notfirst=1;i++;
                }
            }
        }
    }

    i=1;a.n=0;a.notfirst=0;a.is=isempty;
    while(1){
        if(!isRanged(fx-i)||!isRanged(fy-i))  break;
        if(game->game_progress[fx-i][fy-i]==isblack) {
            if(a.notfirst){
                if(a.is==isblack)  a.n++;
                if(a.is==isempty)  {a.is=isblack;a.n++;}
            }
            i++;
        }
        else
        {
            if(game->game_progress[fx-i][fy-i]==iswhite){
                if(!a.notfirst){
                    if(fx+1<21&&fy+1<21&&i>=4) game->black_score[fx+1][fy+1]+=200;//直接连6
                    break;
                }//无空子
                a.n++;
                if(a.is==isblack){
                    if(i>5) {game->black_score[fx-i+a.n-1][fy-i+a.n-1]+=200;break;}//直接连6
                    game->black_score[fx-i+a.n-1][fy-i+a.n-1]+=11*(i-1);break;
                }
                if(a.is==isempty){
                   if(i>5) {game->black_score[fx-i+a.n-1][fy-i+a.n-1]+=200;break;}//直接连6
                   if(fx+1<21&&fy+1<21&&i!=2)
                       game->black_score[fx+1][fy+1]+=5*(i-1);
                   game->black_score[fx-i+a.n-1][fy-i+a.n-1]+=10*(i-1);
                   break;
                }
            }
            if(game->game_progress[fx-i][fy-i]==isempty) {
                a.n++;
                if(a.notfirst){
                        game->black_score[fx-i+a.n-1][fy-i+a.n-1]+=15*(i-1);
                        if(a.n>2) game->black_score[fx-i][fy-i]+=10*(i-1);
                        if(fx+1<21&&fy+1<21&&i!=2)
                            game->black_score[fx+1][fy+1]+=10*(i-1);
                        break;
                }
                else{
                     a.notfirst=1;i++;
                }
            }
        }
    }

}
maxscore GPlayer::maxwhite(){
        maxscore scoremax;
        for(int i=0;i<columnline;i++)
        {
            for(int j=0;j<rowline;j++)
            {
                if(game->white_score[i][j]>scoremax.score){
                    scoremax.score=game->white_score[i][j];
                    scoremax.x=i;scoremax.y=j;
                }
            }
        }
        return scoremax;
}

maxscore GPlayer::maxblack()//获得黑方得分最高的坐标，白方同
{
    maxscore scoremax;
    for(int i=0;i<columnline;i++)
    {
        for(int j=0;j<rowline;j++)
        {
            if(game->black_score[i][j]>scoremax.score){
                scoremax.score=game->black_score[i][j];
                scoremax.x=i;scoremax.y=j;
            }
        }
    }
    return scoremax;
}
void GPlayer::waitsec(int sec)
{
    QEventLoop eventloop;//创建一个事件循环
    QTimer::singleShot(sec, &eventloop, SLOT(quit())); //计时1s
    eventloop.exec();//启动一个事件循环
}

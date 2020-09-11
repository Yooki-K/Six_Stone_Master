﻿#include "gamemodel.h"

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

void Gamemodel::calculatBlack(int fx, int fy)
{
    int i=1;
    while(1){
        if(!isRanged(fy-i))break;
        if(game_progress[fx][fy-i]==isblack) {
            i++;
        }
        else
        {
            if(game_progress[fx][fy-i]==iswhite){
                if(i>=4&&fy+1<21)black_score[fx][fy+1]+=100;
                break;
            }
            if(game_progress[fx][fy-i]==isempty) {
                black_score[fx][fy-i]+=10*i;
                if(fy+1<21&&i!=1)
                    black_score[fx][fy+1]+=10*i;
                break;
            }
        }
    }
   i=1;
    while(1){
        if(!isRanged(fx+i)||!isRanged(fy-i)) break;
        if(game_progress[fx+i][fy-i]==isblack) {
            i++;
        }else
       {
        if(game_progress[fx+i][fy-i]==iswhite){
            if(fx-1>-1&&fy+1<21&&i>=4)black_score[fx-1][fy+1]+=100;
            break;
        }
        if(game_progress[fx+i][fy-i]==isempty) {
            black_score[fx+i][fy-i]+=10*i;
            if(fx-1>-1&&fy+1<21&&i!=1)
            black_score[fx-1][fy+1]+=10*i;
            break;
        }
       }
    }
i=1;
    while(1){

        if(!isRanged(fx+i)) break;
        if(game_progress[fx+i][fy]==isblack) {
            i++;
        }
        else
        {
            if(game_progress[fx+i][fy]==iswhite){
                if(fx-1>-1&&i>=4)black_score[fx-1][fy]+=100;
                break;
            }
            if(game_progress[fx+i][fy]==isempty) {
                black_score[fx+i][fy]+=10*i;
                if(fx-1>-1&&i!=1)
                    black_score[fx-1][fy]+=10*i;
                break;
            }
        }
    }
    i=1;
    while(1){
        if(!isRanged(fy+i)||!isRanged(fx+i))  break;
        if( game_progress[fx+i][fy+i]==isblack) {
            i++;
        }
        else
        {
            if( game_progress[fx+i][fy+i]==iswhite) {
                if(fx-1>-1&&fy-1>-1&&i>=4)black_score[fx-1][fy-1]+=100;
                break;
            }
            if(game_progress[fx+i][fy+i]==isempty) {
                black_score[fx+i][fy+i]+=10*i;
                if(fx-1>-1&&fy-1>-1&&i!=1)
                    black_score[fx-1][fy-1]+=10*i;
                break;
            }
        }

    }
    i=1;
    while(1){
        if(!isRanged(fy+i)) break;
        if(game_progress[fx][fy+i]==isblack) {
            i++;
        }
        else
        {
            if(game_progress[fx][fy+i]==iswhite) {
                if(fy-1>-1&&i>=4)black_score[fx][fy-1]+=100;
                break;
            }
            if(game_progress[fx][fy+i]==isempty) {
                black_score[fx][fy+i]+=10*i;
                if(fy-1>-1&&i!=1)
                    black_score[fx][fy-1]+=10*i;
                break;
            }
        }
    }
    i=1;
    while(1){
        if(!isRanged(fy+i)||!isRanged(fx-i))  break;
        if(game_progress[fx-i][fy+i]==isblack) {
            i++;
        }
        else
        {
            if(game_progress[fx-i][fy+i]==iswhite) {
                if(fx+1<21&&fy-1>-1&&i>=4)black_score[fx+1][fy-1]+=100;
                break;
            }
            if(game_progress[fx-i][fy+i]==isempty) {
                black_score[fx-i][fy+i]+=10*i;
                if(fx+1<21&&fy-1>-1&&i!=1)
                    black_score[fx+1][fy-1]+=10*i;
                break;
            }
        }

    }
    i=1;
    while(1){
        if(!isRanged(fx-i)) break;
        if(game_progress[fx-i][fy]==isblack) {
            i++;
        }
        else
        {
            if(game_progress[fx-i][fy]==iswhite) {
                if(fx+1<21&&i>=4)black_score[fx+1][fy]+=100;
                break;
            }
            if(game_progress[fx-i][fy]==isempty) {
                black_score[fx-i][fy]+=10*i;
                if(fx+1<21&&i!=1)
                    black_score[fx+1][fy]+=10*i;
                break;
            }
        }
    }
    i=1;
    while(1){
        if(!isRanged(fy-i)||!isRanged(fx-i))  break;
        if(game_progress[fx-i][fy-i]==isblack) {
            i++;
        }
        else
        {
            if(game_progress[fx-i][fy-i]==iswhite) {
                if(fx+1<21&&fy+1<21&&i>=4)black_score[fx+1][fy+1]+=100;
                break;
            }
            if(game_progress[fx-i][fy-i]==isempty) {
                black_score[fx-i][fy-i]+=10*i;
                if(fx+1<21&&fy+1<21&&i!=1)
                    black_score[fx+1][fy+1]+=10*i;
                break;
            }
        }
    }
}

void Gamemodel::calculatWhite(int fx, int fy)
{
    int i=1;
    while(1){
        if(!isRanged(fy-i))  break;
        if(game_progress[fx][fy-i]==iswhite) {
            i++;
        }
        else
        {
            if(game_progress[fx][fy-i]==isblack){
                if(fy+1<21&&i>=4)white_score[fx][fy+1]+=100;
                break;
            }
            if(game_progress[fx][fy-i]==isempty) {
                white_score[fx][fy-i]+=10*i;
                if(fy+1<21&&i!=1)
                    white_score[fx][fy+1]+=10*i;
                break;
            }
        }
    }
   i=1;
    while(1){
        if(!isRanged(fx+i)||!isRanged(fy-i))  break;
        if(game_progress[fx+i][fy-i]==iswhite) {
            i++;
        }
        else
        {
            if(game_progress[fx+i][fy-i]==isblack){
                if(fx-1>-1&&fy+1<21&&i>=4)white_score[fx-1][fy+1]+=100;
                break;
            }
            if(game_progress[fx+i][fy-i]==isempty) {
                white_score[fx+i][fy-i]+=10*i;
                if(fx-1>-1&&fy+1<21&&i!=1)
                    white_score[fx-1][fy+1]+=10*i;
                break;
            }
        }

    }
i=1;
    while(1){

        if(!isRanged(fx+i))  break;
        if(game_progress[fx+i][fy]==iswhite) {
            i++;
        }
        else
        {
            if(game_progress[fx+i][fy]==isblack){
                if(fx-1>-1&&i>=4)white_score[fx-1][fy]+=100;
                break;
            }
            if(game_progress[fx+i][fy]==isempty) {
                white_score[fx+i][fy]+=10*i;
                if(fx-1>-1&&i!=1)
                    white_score[fx-1][fy]+=10*i;
                break;
            }
        }
    }
    i=1;
    while(1){
        if(!isRanged(fy+i)||!isRanged(fx+i))  break;
        if( game_progress[fx+i][fy+i]==iswhite) {
            i++;
        }
        else
        {
            if( game_progress[fx+i][fy+i]==isblack) {
                if(fx-1>-1&&fy-1>-1&&i>=4)white_score[fx-1][fy-1]+=100;
                break;
            }
            if(game_progress[fx+i][fy+i]==isempty) {
                white_score[fx+i][fy+i]+=10*i;
                if(fx-1>-1&&fy-1>-1&&i!=1)
                    white_score[fx-1][fy-1]+=10*i;
                break;
            }
        }

    }
    i=1;
    while(1){
        if(!isRanged(fy+i)) break;
        if(game_progress[fx][fy+i]==iswhite) {
            i++;
        }
        else
        {
            if(game_progress[fx][fy+i]==isblack) {
                if(fy-1>-1&&i>=4)white_score[fx][fy-1]+=100;
                break;
            }
            if(game_progress[fx][fy+i]==isempty) {
                white_score[fx][fy+i]+=10*i;
                if(fy-1>-1&&i!=1)
                    white_score[fx][fy-1]+=10*i;
                break;
            }
        }
    }
    i=1;
    while(1){
        if(!isRanged(fy+i)||!isRanged(fx-i))  break;
        if(game_progress[fx-i][fy+i]==iswhite) {
            i++;
        }
        else
        {
            if(game_progress[fx-i][fy+i]==isblack) {
                if(fx+1<21&&fy-1>-1&&i>=4)white_score[fx+1][fy-1]+=100;
                break;
            }
            if(game_progress[fx-i][fy+i]==isempty) {
                white_score[fx-i][fy+i]+=10*i;
                if(fx+1<21&&fy-1>-1&&i!=1)
                    white_score[fx+1][fy-1]+=10*i;
                break;
            }
        }

    }
    i=1;
    while(1){
        if(!isRanged(fx-i)) break;
        if(game_progress[fx-i][fy]==iswhite) {
            i++;
        }
        else
        {
            if(game_progress[fx-i][fy]==isblack) {
                if(fx+1<21&&i>=4)white_score[fx+1][fy]+=100;
                break;
            }
            if(game_progress[fx-i][fy]==isempty) {
                white_score[fx-i][fy]+=10*i;
                if(fx+1<21&&i!=1)
                    white_score[fx+1][fy]+=10*i;
                break;
            }
        }
    }
    i=1;
    while(1){
        if(!isRanged(fy-i)||!isRanged(fx-i))  break;
        if(game_progress[fx-i][fy-i]==iswhite) {
            i++;
        }
        else
        {
            if(game_progress[fx-i][fy-i]==isblack) {
                if(fx+1<21&&fy+1<21&&i>=4)white_score[fx+1][fy+1]+=100;
                break;
            }
            if(game_progress[fx-i][fy-i]==isempty) {
                white_score[fx-i][fy-i]+=10*i;
                if(fx+1<21&&fy+1<21&&i!=1)
                    white_score[fx+1][fy+1]+=10*i;
                break;
            }
        }
    }
}

bool Gamemodel::isRanged(int n)
{
    if(n>=0&&n<columnline) return 1;
    else return 0;
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
void Gamemodel::calculatWhiteUp(int fx, int fy)
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
        if(game_progress[fx][fy-i]==iswhite) {
            if(a.notfirst){
                if(a.is==iswhite)  a.n++;
                if(a.is==isempty)  {a.is=iswhite;a.n++;}
            }
            i++;
        }
        else
        {
            if(game_progress[fx][fy-i]==isblack){
                if(!a.notfirst){
                    if(fy+1<21&&i>=4) white_score[fx][fy+1]+=200;//直接连6
                    break;
                }//无空子
                a.n++;
                if(a.is==iswhite){
                    if(i>5) {white_score[fx][fy-i+a.n-1]+=200;break;}//直接连6
                    white_score[fx][fy-i+a.n-1]+=11*(i-1);break;
                }
                if(a.is==isempty){
                   if(i>5) {white_score[fx][fy-i+a.n-1]+=200;break;}//直接连6
                   if(fy+1<21&&i!=2)
                       white_score[fx][fy+1]+=5*(i-1);
                   white_score[fx][fy-i+a.n-1]+=10*(i-1);
                   break;
                }
            }
            if(game_progress[fx][fy-i]==isempty) {
                a.n++;
                if(a.notfirst){
                        white_score[fx][fy-i+a.n-1]+=15*(i-1);
                        if(a.n>2) white_score[fx][fy-i]+=10*(i-1);
                        if(fy+1<21&&i!=2)
                            white_score[fx][fy+1]+=10*(i-1);
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
       if(game_progress[fx+i][fy-i]==iswhite) {
           if(a.notfirst){
               if(a.is==iswhite)  a.n++;
               if(a.is==isempty)  {a.is=iswhite;a.n++;}
           }
           i++;
       }
       else
       {
           if(game_progress[fx+i][fy-i]==isblack){
               if(!a.notfirst){
                   if(fx-1>-1&&fy+1<21&&i>=4) white_score[fx-1][fy+1]+=200;//直接连6
                   break;
               }//无空子
               a.n++;
               if(a.is==iswhite){
                   if(i>5) {white_score[fx+i-a.n+1][fy-i+a.n-1]+=200;break;}//直接连6
                   white_score[fx+i-a.n+1][fy-i+a.n-1]+=11*(i-1);break;
               }
               if(a.is==isempty){
                  if(i>5) {white_score[fx+i-a.n+1][fy-i+a.n-1]+=200;break;}//直接连6
                  if(fx-1>-1&&fy+1<21&&i!=2)
                      white_score[fx-1][fy+1]+=5*(i-1);
                  white_score[fx+i-a.n+1][fy-i+a.n-1]+=10*(i-1);
                  break;
               }
           }
           if(game_progress[fx+i][fy-i]==isempty) {
               a.n++;
               if(a.notfirst){
                       white_score[fx+i-a.n+1][fy-i+a.n-1]+=15*(i-1);
                       if(a.n>2) white_score[fx+i][fy-i]+=10*(i-1);
                       if(fx-1>-1&&fy+1<21&&i!=2)
                           white_score[fx-1][fy+1]+=10*(i-1);
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
       if(game_progress[fx+i][fy]==iswhite) {
           if(a.notfirst){
               if(a.is==iswhite)  a.n++;
               if(a.is==isempty)  {a.is=iswhite;a.n++;}
           }
           i++;
       }
       else
       {
           if(game_progress[fx+i][fy]==isblack){
               if(!a.notfirst){
                   if(fx-1>-1&&i>=4) white_score[fx+i][fy]+=200;//直接连6
                   break;
               }//无空子
               a.n++;
               if(a.is==iswhite){
                   if(i>5) {white_score[fx+i-a.n+1][fy]+=200;break;}//直接连6
                   white_score[fx+i-a.n+1][fy]+=11*(i-1);break;
               }
               if(a.is==isempty){
                  if(i>5) {white_score[fx+i-a.n+1][fy]+=200;break;}//直接连6
                  if(fx-1>-1&&i!=2)
                      white_score[fx-1][fy]+=5*(i-1);
                  white_score[fx+i-a.n+1][fy]+=10*(i-1);
                  break;
               }
           }
           if(game_progress[fx+i][fy]==isempty) {
               a.n++;
               if(a.notfirst){
                       white_score[fx+i-a.n+1][fy]+=15*(i-1);
                       if(a.n>2) white_score[fx+i][fy]+=10*(i-1);
                       if(fx-1>-1&&i!=2)
                           white_score[fx-1][fy]+=10*(i-1);
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
        if(game_progress[fx+i][fy+i]==iswhite) {
            if(a.notfirst){
                if(a.is==iswhite)  a.n++;
                if(a.is==isempty)  {a.is=iswhite;a.n++;}
            }
            i++;
        }
        else
        {
            if(game_progress[fx+i][fy+i]==isblack){
                if(!a.notfirst){
                    if(fx-1>-1&&fy-1>-1&&i>=4) white_score[fx-1][fy-1]+=200;//直接连6
                    break;
                }//无空子
                a.n++;
                if(a.is==iswhite){
                    if(i>5) {white_score[fx+i-a.n+1][fy+i-a.n+1]+=200;break;}//直接连6
                    white_score[fx+i-a.n+1][fy+i-a.n+1]+=11*(i-1);break;
                }
                if(a.is==isempty){
                   if(i>5) {white_score[fx+i-a.n+1][fy+i-a.n+1]+=200;break;}//直接连6
                   if(fx-1>-1&&fy-1>-1&&i!=2)
                       white_score[fx-1][fy-1]+=5*(i-1);
                   white_score[fx+i-a.n+1][fy+i-a.n+1]+=10*(i-1);
                   break;
                }
            }
            if(game_progress[fx+i][fy+i]==isempty) {
                a.n++;
                if(a.notfirst){
                        white_score[fx+i-a.n+1][fy+i-a.n+1]+=15*(i-1);
                        if(a.n>2) white_score[fx+i][fy+i]+=10*(i-1);
                        if(fx-1>-1&&fy-1>-1&&i!=2)
                            white_score[fx-1][fy-1]+=10*(i-1);
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
        if(game_progress[fx][fy+i]==iswhite) {
            if(a.notfirst){
                if(a.is==iswhite)  a.n++;
                if(a.is==isempty)  {a.is=iswhite;a.n++;}
            }
            i++;
        }
        else
        {
            if(game_progress[fx][fy+i]==isblack){
                if(!a.notfirst){
                    if(fy-1>-1&&i>=4) white_score[fx][fy-1]+=200;//直接连6
                    break;
                }//无空子
                a.n++;
                if(a.is==iswhite){
                    if(i>5) {white_score[fx][fy+i-a.n+1]+=200;break;}//直接连6
                    white_score[fx][fy+i-a.n+1]+=11*(i-1);break;
                }
                if(a.is==isempty){
                   if(i>5) {white_score[fx][fy+i-a.n+1]+=200;break;}//直接连6
                   if(fy-1>-1&&i!=2)
                       white_score[fx][fy-1]+=5*(i-1);
                   white_score[fx][fy+i-a.n+1]+=10*(i-1);
                   break;
                }
            }
            if(game_progress[fx][fy+i]==isempty) {
                a.n++;
                if(a.notfirst){
                        white_score[fx][fy+i-a.n+1]+=15*(i-1);
                        if(a.n>2) white_score[fx][fy+i]+=10*(i-1);
                        if(fy-1>-1&&i!=2)
                            white_score[fx][fy-1]+=10*(i-1);
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
        if(game_progress[fx-i][fy+i]==iswhite) {
            if(a.notfirst){
                if(a.is==iswhite)  a.n++;
                if(a.is==isempty)  {a.is=iswhite;a.n++;}
            }
            i++;
        }
        else
        {
            if(game_progress[fx-i][fy+i]==isblack){
                if(!a.notfirst){
                    if(fx+1<21&&fy-1>-1&&i>=4) white_score[fx+1][fy-1]+=200;//直接连6
                    break;
                }//无空子
                a.n++;
                if(a.is==iswhite){
                    if(i>5) {white_score[fx-i+a.n-1][fy+i-a.n+1]+=200;break;}//直接连6
                    white_score[fx-i+a.n-1][fy+i-a.n+1]+=11*(i-1);break;
                }
                if(a.is==isempty){
                   if(i>5) {white_score[fx-i+a.n-1][fy+i-a.n+1]+=200;break;}//直接连6
                   if(fx+1<21&&fy-1>-1&&i!=2)
                       white_score[fx+1][fy-1]+=5*(i-1);
                   white_score[fx-i+a.n-1][fy+i-a.n+1]+=10*(i-1);
                   break;
                }
            }
            if(game_progress[fx-i][fy+i]==isempty) {
                a.n++;
                if(a.notfirst){
                        white_score[fx-i+a.n-1][fy+i-a.n+1]+=15*(i-1);
                        if(a.n>2) white_score[fx-i][fy+i]+=10*(i-1);
                        if(fx+1<21&&fy-1>-1&&i!=2)
                            white_score[fx+1][fy-1]+=10*(i-1);
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
        if(game_progress[fx-i][fy]==iswhite) {
            if(a.notfirst){
                if(a.is==iswhite)  a.n++;
                if(a.is==isempty)  {a.is=iswhite;a.n++;}
            }
            i++;
        }
        else
        {
            if(game_progress[fx-i][fy]==isblack){
                if(!a.notfirst){
                    if(fx+1<21&&i>=4) white_score[fx+1][fy]+=200;//直接连6
                    break;
                }//无空子
                a.n++;
                if(a.is==iswhite){
                    if(i>5) {white_score[fx-i+a.n-1][fy]+=200;break;}//直接连6
                    white_score[fx-i+a.n-1][fy]+=11*(i-1);break;
                }
                if(a.is==isempty){
                   if(i>5) {white_score[fx-i+a.n-1][fy]+=200;break;}//直接连6
                   if(fx+1<21&&i!=2)
                       white_score[fx+1][fy]+=5*(i-1);
                   white_score[fx-i+a.n-1][fy]+=10*(i-1);
                   break;
                }
            }
            if(game_progress[fx-i][fy]==isempty) {
                a.n++;
                if(a.notfirst){
                        white_score[fx-i+a.n-1][fy]+=15*(i-1);
                        if(a.n>2) white_score[fx-i][fy]+=10*(i-1);
                        if(fx+1<21&&i!=2)
                            white_score[fx+1][fy]+=10*(i-1);
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
        if(game_progress[fx-i][fy-i]==iswhite) {
            if(a.notfirst){
                if(a.is==iswhite)  a.n++;
                if(a.is==isempty)  {a.is=iswhite;a.n++;}
            }
            i++;
        }
        else
        {
            if(game_progress[fx-i][fy-i]==isblack){
                if(!a.notfirst){
                    if(fx+1<21&&fy+1<21&&i>=4) white_score[fx+1][fy+1]+=200;//直接连6
                    break;
                }//无空子
                a.n++;
                if(a.is==iswhite){
                    if(i>5) {white_score[fx-i+a.n-1][fy-i+a.n-1]+=200;break;}//直接连6
                    white_score[fx-i+a.n-1][fy-i+a.n-1]+=11*(i-1);break;
                }
                if(a.is==isempty){
                   if(i>5) {white_score[fx-i+a.n-1][fy-i+a.n-1]+=200;break;}//直接连6
                   if(fx+1<21&&fy+1<21&&i!=2)
                       white_score[fx+1][fy+1]+=5*(i-1);
                   white_score[fx-i+a.n-1][fy-i+a.n-1]+=10*(i-1);
                   break;
                }
            }
            if(game_progress[fx-i][fy-i]==isempty) {
                a.n++;
                if(a.notfirst){
                        white_score[fx-i+a.n-1][fy-i+a.n-1]+=15*(i-1);
                        if(a.n>2) white_score[fx-i][fy-i]+=10*(i-1);
                        if(fx+1<21&&fy+1<21&&i!=2)
                            white_score[fx+1][fy+1]+=10*(i-1);
                        break;
                }
                else{
                     a.notfirst=1;i++;
                }
            }
        }
    }

}

void Gamemodel::calculatBlackUp(int fx, int fy)
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
        if(game_progress[fx][fy-i]==isblack) {
            if(a.notfirst){
                if(a.is==isblack)  a.n++;
                if(a.is==isempty)  {a.is=isblack;a.n++;}
            }
            i++;
        }
        else
        {
            if(game_progress[fx][fy-i]==iswhite){
                if(!a.notfirst){
                    if(fy+1<21&&i>=4) black_score[fx][fy+1]+=200;//直接连6
                    break;
                }//无空子
                a.n++;
                if(a.is==isblack){
                    if(i>5) {black_score[fx][fy-i+a.n-1]+=200;break;}//直接连6
                    black_score[fx][fy-i+a.n-1]+=11*(i-1);break;
                }
                if(a.is==isempty){
                   if(i>5) {black_score[fx][fy-i+a.n-1]+=200;break;}//直接连6
                   if(fy+1<21&&i!=2)
                       black_score[fx][fy+1]+=5*(i-1);
                   black_score[fx][fy-i+a.n-1]+=10*(i-1);
                   break;
                }
            }
            if(game_progress[fx][fy-i]==isempty) {
                a.n++;
                if(a.notfirst){
                        black_score[fx][fy-i+a.n-1]+=15*(i-1);
                        if(a.n>2) black_score[fx][fy-i]+=10*(i-1);
                        if(fy+1<21&&i!=2)
                            black_score[fx][fy+1]+=10*(i-1);
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
       if(game_progress[fx+i][fy-i]==isblack) {
           if(a.notfirst){
               if(a.is==isblack)  a.n++;
               if(a.is==isempty)  {a.is=isblack;a.n++;}
           }
           i++;
       }
       else
       {
           if(game_progress[fx+i][fy-i]==iswhite){
               if(!a.notfirst){
                   if(fx-1>-1&&fy+1<21&&i>=4) black_score[fx-1][fy+1]+=200;//直接连6
                   break;
               }//无空子
               a.n++;
               if(a.is==isblack){
                   if(i>5) {black_score[fx+i-a.n+1][fy-i+a.n-1]+=200;break;}//直接连6
                   black_score[fx+i-a.n+1][fy-i+a.n-1]+=11*(i-1);break;
               }
               if(a.is==isempty){
                  if(i>5) {black_score[fx+i-a.n+1][fy-i+a.n-1]+=200;break;}//直接连6
                  if(fx-1>-1&&fy+1<21&&i!=2)
                      black_score[fx-1][fy+1]+=5*(i-1);
                  black_score[fx+i-a.n+1][fy-i+a.n-1]+=10*(i-1);
                  break;
               }
           }
           if(game_progress[fx+i][fy-i]==isempty) {
               a.n++;
               if(a.notfirst){
                       black_score[fx+i-a.n+1][fy-i+a.n-1]+=15*(i-1);
                       if(a.n>2) black_score[fx+i][fy-i]+=10*(i-1);
                       if(fx-1>-1&&fy+1<21&&i!=2)
                           black_score[fx-1][fy+1]+=10*(i-1);
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
       if(game_progress[fx+i][fy]==isblack) {
           if(a.notfirst){
               if(a.is==isblack)  a.n++;
               if(a.is==isempty)  {a.is=isblack;a.n++;}
           }
           i++;
       }
       else
       {
           if(game_progress[fx+i][fy]==iswhite){
               if(!a.notfirst){
                   if(fx-1>-1&&i>=4) black_score[fx-1][fy]+=200;//直接连6
                   break;
               }//无空子
               a.n++;
               if(a.is==isblack){
                   if(i>5) {black_score[fx+i-a.n+1][fy]+=200;break;}//直接连6
                   black_score[fx+i-a.n+1][fy]+=11*(i-1);break;
               }
               if(a.is==isempty){
                  if(i>5) {black_score[fx+i-a.n+1][fy]+=200;break;}//直接连6
                  if(fx-1>-1&&i!=2)
                      black_score[fx-1][fy]+=5*(i-1);
                  black_score[fx+i-a.n+1][fy]+=10*(i-1);
                  break;
               }
           }
           if(game_progress[fx+i][fy]==isempty) {
               a.n++;
               if(a.notfirst){
                       black_score[fx+i-a.n+1][fy]+=15*(i-1);
                       if(a.n>2) black_score[fx+i][fy]+=10*(i-1);
                       if(fx-1>-1&&i!=2)
                           black_score[fx-1][fy]+=10*(i-1);
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
        if(game_progress[fx+i][fy+i]==isblack) {
            if(a.notfirst){
                if(a.is==isblack)  a.n++;
                if(a.is==isempty)  {a.is=isblack;a.n++;}
            }
            i++;
        }
        else
        {
            if(game_progress[fx+i][fy+i]==iswhite){
                if(!a.notfirst){
                    if(fx-1>-1&&fy-1>-1&&i>=4) black_score[fx-1][fy-1]+=200;//直接连6
                    break;
                }//无空子
                a.n++;
                if(a.is==isblack){
                    if(i>5) {black_score[fx+i-a.n+1][fy+i-a.n+1]+=200;break;}//直接连6
                    black_score[fx+i-a.n+1][fy+i-a.n+1]+=11*(i-1);break;
                }
                if(a.is==isempty){
                   if(i>5) {black_score[fx+i-a.n+1][fy+i-a.n+1]+=200;break;}//直接连6
                   if(fx-1>-1&&fy-1>-1&&i!=2)
                       black_score[fx-1][fy-1]+=5*(i-1);
                   black_score[fx+i-a.n+1][fy+i-a.n+1]+=10*(i-1);
                   break;
                }
            }
            if(game_progress[fx+i][fy+i]==isempty) {
                a.n++;
                if(a.notfirst){
                        black_score[fx+i-a.n+1][fy+i-a.n+1]+=15*(i-1);
                        if(a.n>2) black_score[fx+i][fy+i]+=10*(i-1);
                        if(fx-1>-1&&fy-1>-1&&i!=2)
                            black_score[fx-1][fy-1]+=10*(i-1);
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
        if(game_progress[fx][fy+i]==isblack) {
            if(a.notfirst){
                if(a.is==isblack)  a.n++;
                if(a.is==isempty)  {a.is=isblack;a.n++;}
            }
            i++;
        }
        else
        {
            if(game_progress[fx][fy+i]==iswhite){
                if(!a.notfirst){
                    if(fy-1>-1&&i>=4) black_score[fx][fy-1]+=200;//直接连6
                    break;
                }//无空子
                a.n++;
                if(a.is==isblack){
                    if(i>5) {black_score[fx][fy+i-a.n+1]+=200;break;}//直接连6
                    black_score[fx][fy+i-a.n+1]+=11*(i-1);break;
                }
                if(a.is==isempty){
                   if(i>5) {black_score[fx][fy+i-a.n+1]+=200;break;}//直接连6
                   if(fy-1>-1&&i!=2)
                       black_score[fx][fy-1]+=5*(i-1);
                   black_score[fx][fy+i-a.n+1]+=10*(i-1);
                   break;
                }
            }
            if(game_progress[fx][fy+i]==isempty) {
                a.n++;
                if(a.notfirst){
                        black_score[fx][fy+i-a.n+1]+=15*(i-1);
                        if(a.n>2) black_score[fx][fy+i]+=10*(i-1);
                        if(fy-1>-1&&i!=2)
                            black_score[fx][fy-1]+=10*(i-1);
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
        if(game_progress[fx-i][fy+i]==isblack) {
            if(a.notfirst){
                if(a.is==isblack)  a.n++;
                if(a.is==isempty)  {a.is=isblack;a.n++;}
            }
            i++;
        }
        else
        {
            if(game_progress[fx-i][fy+i]==iswhite){
                if(!a.notfirst){
                    if(fx+1<21&&fy-1>-1&&i>=4) black_score[fx+1][fy-1]+=200;//直接连6
                    break;
                }//无空子
                a.n++;
                if(a.is==isblack){
                    if(i>5) {black_score[fx-i+a.n-1][fy+i-a.n+1]+=200;break;}//直接连6
                    black_score[fx-i+a.n-1][fy+i-a.n+1]+=11*(i-1);break;
                }
                if(a.is==isempty){
                   if(i>5) {black_score[fx-i+a.n-1][fy+i-a.n+1]+=200;break;}//直接连6
                   if(fx+1<21&&fy-1>-1&&i!=2)
                       black_score[fx+1][fy-1]+=5*(i-1);
                   black_score[fx-i+a.n-1][fy+i-a.n+1]+=10*(i-1);
                   break;
                }
            }
            if(game_progress[fx-i][fy+i]==isempty) {
                a.n++;
                if(a.notfirst){
                        black_score[fx-i+a.n-1][fy+i-a.n+1]+=15*(i-1);
                        if(a.n>2) black_score[fx-i][fy+i]+=10*(i-1);
                        if(fx+1<21&&fy-1>-1&&i!=2)
                            black_score[fx+1][fy-1]+=10*(i-1);
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
        if(game_progress[fx-i][fy]==isblack) {
            if(a.notfirst){
                if(a.is==isblack)  a.n++;
                if(a.is==isempty)  {a.is=isblack;a.n++;}
            }
            i++;
        }
        else
        {
            if(game_progress[fx-i][fy]==iswhite){
                if(!a.notfirst){
                    if(fx+1<21&&i>=4) black_score[fx+1][fy]+=200;//直接连6
                    break;
                }//无空子
                a.n++;
                if(a.is==isblack){
                    if(i>5) {black_score[fx-i+a.n-1][fy]+=200;break;}//直接连6
                    black_score[fx-i+a.n-1][fy]+=11*(i-1);break;
                }
                if(a.is==isempty){
                   if(i>5) {black_score[fx-i+a.n-1][fy]+=200;break;}//直接连6
                   if(fx+1<21&&i!=2)
                       black_score[fx+1][fy]+=5*(i-1);
                   black_score[fx-i+a.n-1][fy]+=10*(i-1);
                   break;
                }
            }
            if(game_progress[fx-i][fy]==isempty) {
                a.n++;
                if(a.notfirst){
                        black_score[fx-i+a.n-1][fy]+=15*(i-1);
                        if(a.n>2) black_score[fx-i][fy]+=10*(i-1);
                        if(fx+1<21&&i!=2)
                            black_score[fx+1][fy]+=10*(i-1);
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
        if(game_progress[fx-i][fy-i]==isblack) {
            if(a.notfirst){
                if(a.is==isblack)  a.n++;
                if(a.is==isempty)  {a.is=isblack;a.n++;}
            }
            i++;
        }
        else
        {
            if(game_progress[fx-i][fy-i]==iswhite){
                if(!a.notfirst){
                    if(fx+1<21&&fy+1<21&&i>=4) black_score[fx+1][fy+1]+=200;//直接连6
                    break;
                }//无空子
                a.n++;
                if(a.is==isblack){
                    if(i>5) {black_score[fx-i+a.n-1][fy-i+a.n-1]+=200;break;}//直接连6
                    black_score[fx-i+a.n-1][fy-i+a.n-1]+=11*(i-1);break;
                }
                if(a.is==isempty){
                   if(i>5) {black_score[fx-i+a.n-1][fy-i+a.n-1]+=200;break;}//直接连6
                   if(fx+1<21&&fy+1<21&&i!=2)
                       black_score[fx+1][fy+1]+=5*(i-1);
                   black_score[fx-i+a.n-1][fy-i+a.n-1]+=10*(i-1);
                   break;
                }
            }
            if(game_progress[fx-i][fy-i]==isempty) {
                a.n++;
                if(a.notfirst){
                        black_score[fx-i+a.n-1][fy-i+a.n-1]+=15*(i-1);
                        if(a.n>2) black_score[fx-i][fy-i]+=10*(i-1);
                        if(fx+1<21&&fy+1<21&&i!=2)
                            black_score[fx+1][fy+1]+=10*(i-1);
                        break;
                }
                else{
                     a.notfirst=1;i++;
                }
            }
        }
    }

}


#include "jiemian.h"
#include<QDebug>
Jiemian::Jiemian(QWidget *parent) : QWidget(parent)
{
    game=new Gamemodel;
    setMouseTracking(true);//开启鼠标监听
    Sound_effect = new QMediaPlayer;
    Sound_effect->setMedia(QUrl("qrc:/new/background/reso/down.wav"));
    Sound_effect->setVolume(200);


}

void Jiemian::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(QPen(QColor("black")));
    painter.setRenderHint(QPainter:: Antialiasing, true);  //设置渲染,启动反锯齿
    painter.setBrush(QColor(230,255,255));
    painter.drawRect(margin-2*r,margin-2*r,one*20+4*r,one*20+4*r);//画轮廓

    for(int i=0;i<rowline;i++){
        painter.drawLine(margin,margin+i*one,margin+20*one,margin+i*one);
    }
    for(int i=0;i<columnline;i++){
        painter.drawLine(margin+i*one,margin,margin+i*one,margin+20*one);
    }//画棋牌
    if(isselected){
        if(game->Gameflags){
            painter.setBrush(QColor("black"));
        }else{
            painter.setBrush(QColor("white"));
        }
        QPoint ing(clickx*one+margin,clicky*one+margin);
        painter.drawEllipse(ing,r,r);
    }//鼠标监听
    for(int i=0;i<columnline;i++)//行
    {
        for(int j=0;j<rowline;j++)//添加21列
        {
            switch (game->game_progress[i][j]) {
            case iswhite:
                painter.setBrush(QColor("white"));
                break;
            case isblack:
                painter.setBrush(QColor("black"));
                break;
            default:
                continue;
            }
            QPoint qizi(i*one+margin,j*one+margin);
            painter.drawEllipse(qizi,r,r);
        }
    }//画棋子
    if(game->state==win){
        painter.setPen(QPen(QBrush(QColor("red")),8));
        switch (game->derect) {
        case 0:
            painter.drawLine(clickx*one+margin,clicky*one+margin,clickx*one+margin,(clicky-5)*one+margin);
            break;
        case 1:
            painter.drawLine(clickx*one+margin,clicky*one+margin,(clickx+5)*one+margin,(clicky-5)*one+margin);
            break;
        case 2:
            painter.drawLine(clickx*one+margin,clicky*one+margin,(clickx+5)*one+margin,clicky*one+margin);
            break;
        case 3:
            painter.drawLine(clickx*one+margin,clicky*one+margin,(clickx+5)*one+margin,(clicky+5)*one+margin);
            break;
        case 4:
            painter.drawLine(clickx*one+margin,clicky*one+margin,clickx*one+margin,(clicky+5)*one+margin);
            break;
        case 5:
            painter.drawLine(clickx*one+margin,clicky*one+margin,(clickx-5)*one+margin,(clicky+5)*one+margin);
            break;
        case 6:
            painter.drawLine(clickx*one+margin,clicky*one+margin,(clickx-5)*one+margin,clicky*one+margin);
            break;
        case 7:
            painter.drawLine(clickx*one+margin,clicky*one+margin,(clickx-5)*one+margin,(clicky-5)*one+margin);
            break;
        default:
            break;
        }
    }

}

void Jiemian::mouseMoveEvent(QMouseEvent *event)
{
    if(game->state!=playing) return;
    x=event->x();
    y=event->y();
    isselected=0;
    int minx;
    int miny;
    if((x-margin)%one>one-((x-margin)%one)){
        minx= ((x-margin)%one)-one;
    }else{
        minx=(x-margin)%one>one;
    }
    if((y-margin)%one>one-((y-margin)%one)){
        miny= ((y-margin)%one)-one;
    }else{
        miny=(y-margin)%one;
    }
    if(minx*minx+miny*miny<r*r){
        clickx=(x-margin-minx)/one;
        clicky=(y-margin-miny)/one;
    if(clickx<0||clicky<0||clickx>20||clicky>20) return;//防止程序异常
        if(game->game_progress[clickx][clicky]==isempty){
                isselected=1;
        }
    }


}

void Jiemian::mouseReleaseEvent(QMouseEvent *)
{
    if(isselected){
        game->game_progress[clickx][clicky]=(what)game->Gameflags;
        game->Gameflags=!game->Gameflags;
        Sound_effect->play();//播放落子音效
        game->state=game->GameEnd(clickx,clicky);
        if(game->state==win){

        }
        update();
    }//落点存储


}
Jiemian::~Jiemian(){
    delete game;
}

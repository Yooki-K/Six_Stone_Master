#include "jiemian.h"
#include<QDebug>
Jiemian::Jiemian(QWidget *parent) : QWidget(parent)
{
    game=new Gamemodel;
    setMouseTracking(true);
    for(int i=0;i<21;i++)
    {
        game_progress.push_back(std::vector<what>());//不断加行
    }
    for(int i=0;i<game_progress.size();i++)//行
    {
        for(int j=0;j<21;j++)//添加21列
        {
            game_progress[i].push_back(isempty);
        }
    }

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
        if(1){//game->flagsing
            painter.setBrush(QColor("black"));
        }else{
            painter.setBrush(QColor("white"));
        }
        QPoint ing(clickx*one+margin,clicky*one+margin);
        painter.drawEllipse(ing,r,r);
    }
}

void Jiemian::mouseMoveEvent(QMouseEvent *event)
{
    x=event->x();
    y=event->y();
    qDebug()<<x<<" "<<y;

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
        qDebug()<<clickx<<" "<<clicky;
    if(clickx<0||clicky<0||clickx>20||clicky>20) return;
        if(game_progress[clickx][clicky]==isempty){
                isselected=1;
        }
    }

}
Jiemian::~Jiemian(){
    delete game;
}

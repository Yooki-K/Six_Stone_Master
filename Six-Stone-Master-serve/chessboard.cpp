#include "chessboard.h"
#include "ui_chessboard.h"
#include"ai.h"
//#include<QDebug>
Chessboard::Chessboard(QWidget *parent, Gamemodel *game) :
    QWidget(parent),game(game),
    ui(new Ui::Chessboard)
{
    move(200,50);
    ui->setupUi(this);
    if(parent==0){
        ui->widget->move(WIDTH-ui->widget->width()-100,HEIGHT/2-ui->widget->height()/2);
    }
    if(!hasMouseTracking())
    setMouseTracking(true);
    Sound_effect = new QMediaPlayer(this);
    Sound_effect->setMedia(QUrl("qrc:/new/background/reso/down.wav"));
    Sound_effect->setVolume(200);
    if(game->isonline!=-1)
    {
        ui->flagchoose->close();
    }
    else{
        ui->meslist->close();
        ui->btsend->close();
        ui->lineEdit->close();
    }
}


Chessboard::~Chessboard()
{
    delete ui;
}


void Chessboard::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(QPen(QColor("black")));
    painter.setRenderHint(QPainter:: Antialiasing, true);  //设置渲染,启动反锯齿
    painter.setBrush(QColor(230,255,255));
    if(parent()==0)
    {
        painter.drawRect(200+margin-2*r,50+margin-2*r,one*20+4*r,one*20+4*r);//画轮廓

        for(int i=0;i<rowline;i++){
            painter.drawLine(200+margin,50+margin+i*one,200+margin+20*one,50+margin+i*one);
        }
        for(int i=0;i<columnline;i++){
            painter.drawLine(200+margin+i*one,50+margin,200+margin+i*one,50+margin+20*one);
        }//画棋牌
        //    painter.setPen(QPen(QColor("green")));
        //    painter.setFont(QFont("微软雅黑",10,700,false));
        //    for(int i=0;i<rowline;i++){
        //        for(int j=0;j<columnline;j++){
        //            painter.drawText(QPoint(margin+one*i,margin+one*j),QString::number( game->black_score[i][j]));
        //        }
        //    }
        //    painter.setPen(QPen(QColor("blue")));
        //    painter.setFont(QFont("微软雅黑",10,700,false));
        //    for(int i=0;i<rowline;i++){
        //        for(int j=0;j<columnline;j++){
        //            painter.drawText(QPoint(margin+one*i,margin+one*j-10),QString::number( game->white_score[i][j]));
        //        }
        //    }//绘制分数（调试时用）
        painter.setPen(QPen(QColor("blue")));
        if(isselected){
            if( game->Gameflags){
                painter.setBrush(QColor("black"));
            }else{
                painter.setBrush(QColor("white"));
            }
            QPoint ing(200+clickx*one+margin,50+clicky*one+margin);
            painter.drawEllipse(ing,r,r);
        }//鼠标监听
        for(int i=0;i<columnline;i++)//行
        {
            for(int j=0;j<rowline;j++)//添加21列
            {
                switch ( game->game_progress[i][j]) {
                case iswhite:
                    painter.setBrush(QColor("white"));
                    break;
                case isblack:
                    painter.setBrush(QColor("black"));
                    break;
                default:
                    continue;
                }
                QPoint qizi(200+i*one+margin,50+j*one+margin);
                painter.drawEllipse(qizi,r,r);
            }
        }//画棋子
        if(  game->backx>0&&  game->backy>0){
            painter.setBrush(QColor("red"));
            QPoint qizi(  200+game->backx*one+margin,  50+game->backy*one+margin);
            painter.drawEllipse(qizi,10,10);
        }//画最新棋子
        if(  game->state==win){
            painter.setPen(QPen(QBrush(QColor("red")),8));
            switch ( game->derect) {
            case 0:
                painter.drawLine( 200+game->winx*one+margin, 50+game->winy*one+margin, 200+game->winx*one+margin,50+( game->winy-5)*one+margin);
                break;
            case 3:
                painter.drawLine( 200+game->winx*one+margin, 50+game->winy*one+margin,200+( game->winx-5)*one+margin,50+( game->winy+5)*one+margin);
                break;
            case 2:
                painter.drawLine( 200+game->winx*one+margin, 50+game->winy*one+margin,200+( game->winx-5)*one+margin, 50+game->winy*one+margin);
                break;
            case 1:
                painter.drawLine( 200+game->winx*one+margin, 50+game->winy*one+margin,200+( game->winx-5)*one+margin,50+( game->winy-5)*one+margin);
                break;
            default:
                break;
            }
        }//游戏赢了画红线
    }
    else
    {
        painter.drawRect(margin-2*r,margin-2*r,one*20+4*r,one*20+4*r);//画轮廓

        for(int i=0;i<rowline;i++){
            painter.drawLine(margin,margin+i*one,margin+20*one,margin+i*one);
        }
        for(int i=0;i<columnline;i++){
            painter.drawLine(margin+i*one,margin,margin+i*one,margin+20*one);
        }//画棋牌
        //    painter.setPen(QPen(QColor("green")));
        //    painter.setFont(QFont("微软雅黑",10,700,false));
        //    for(int i=0;i<rowline;i++){
        //        for(int j=0;j<columnline;j++){
        //            painter.drawText(QPoint(margin+one*i,margin+one*j),QString::number( game->black_score[i][j]));
        //        }
        //    }
        //    painter.setPen(QPen(QColor("blue")));
        //    painter.setFont(QFont("微软雅黑",10,700,false));
        //    for(int i=0;i<rowline;i++){
        //        for(int j=0;j<columnline;j++){
        //            painter.drawText(QPoint(margin+one*i,margin+one*j-10),QString::number( game->white_score[i][j]));
        //        }
        //    }//绘制分数（调试时用）
        painter.setPen(QPen(QColor("blue")));
        if(isselected){
            if( game->Gameflags){
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
                switch ( game->game_progress[i][j]) {
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
        if(  game->backx>0&&  game->backy>0){
            painter.setBrush(QColor("red"));
            QPoint qizi(  game->backx*one+margin,  game->backy*one+margin);
            painter.drawEllipse(qizi,10,10);
        }//画最新棋子
        if(  game->state==win){
            painter.setPen(QPen(QBrush(QColor("red")),8));
            switch ( game->derect) {
            case 0:
                painter.drawLine( game->winx*one+margin, game->winy*one+margin, game->winx*one+margin,( game->winy-5)*one+margin);
                break;
            case 3:
                painter.drawLine( game->winx*one+margin, game->winy*one+margin,( game->winx-5)*one+margin,( game->winy+5)*one+margin);
                break;
            case 2:
                painter.drawLine( game->winx*one+margin, game->winy*one+margin,( game->winx-5)*one+margin, game->winy*one+margin);
                break;
            case 1:
                painter.drawLine( game->winx*one+margin, game->winy*one+margin,( game->winx-5)*one+margin,( game->winy-5)*one+margin);
                break;
            default:
                break;
            }
        }//游戏赢了画红线
    }

}


void Chessboard::mouseMoveEvent(QMouseEvent *event)
{
    if(  game->state!=playing) return;
    if(  game->type==AA)return;
    if(  game->type==MA&&game->Gameflags!=game->player1->myflag) return;
    x=event->x();
    y=event->y();
    if(x<margin||y<margin||x>margin+20*one||y>margin+20*one)return;
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
        if(  game->game_progress[clickx][clicky]==isempty){
                isselected=1;
                update();
        }
    }

}

void Chessboard::mouseReleaseEvent(QMouseEvent *)
{
    if(  game->state!=playing) return;
    if(  game->type==AA) return;//AA模式
    if(  game->type==MA&&game->Gameflags!=game->player1->myflag) return;//AI下棋时不监听
    if(isselected){
        emit mouseRelease();
    }
}

void Chessboard::closeflag()
{
    ui->flagchoose->close();
}


void Chessboard::on_btwhite_clicked()
{
    if(  game->type==MM)
    {
          game->player1=new GPlayer(0,game,this);
          game->player2=new GPlayer(1,game,this);
    }
    else{
        if(  game->type==MA){
              game->player1=new GPlayer(0,game,this);
              game->player2=new AI(1,this,game,game->AItype);
        }else{
              game->player1=new AI(0,this,game);
              game->player2=new AI(1,this,game);
        }
    }
    setMouseTracking(true);//开启鼠标监听
    ui->flagchoose->close();
    game->start();
}

void Chessboard::on_btblack_clicked()
{
    if(  game->type==MM)
    {
          game->player1=new GPlayer(1,game,this);
          game->player2=new GPlayer(0,game,this);
    }
    else{
        if(  game->type==MA){
              game->player1=new GPlayer(1,game,this);
              game->player2=new AI(0,this,game, game->AItype);
        }else{
              game->player1=new AI(1,this,game);
              game->player2=new AI(0,this,game);
        }
    }
    setMouseTracking(true);//开启鼠标监听
    ui->flagchoose->close();
    game->start();
}

void Chessboard::on_btback_clicked()
{
    if(game->Gameflags==game->player1->myflag)
        game->backStep(game->player1);
    else
        game->backStep(game->player2);
}


void Chessboard::on_btgvup_clicked()
{
    if(game->Gameflags==game->player1->myflag)
        game->giveup(game->player1);
    else
        game->giveup(game->player2);
}

void Chessboard::on_btsend_clicked()
{
    if(!ui->lineEdit->text().isEmpty())
    {
        emit sendmesschat(QTime::currentTime().toString()+' '+game->player1->name+':'+ui->lineEdit->text());
        ui->meslist->addItem(QTime::currentTime().toString()+' '+game->player1->name+':'+ui->lineEdit->text());
        ui->lineEdit->clear();
    }
}

void Chessboard::updatechat(QString mes)
{
    ui->meslist->addItem(mes);
}


void Chessboard::on_lineEdit_returnPressed()
{
    if(!ui->lineEdit->text().isEmpty())
    {
        emit sendmesschat(QTime::currentTime().toString()+' '+game->player1->name+':'+ui->lineEdit->text());
        ui->meslist->addItem(QTime::currentTime().toString()+' '+game->player1->name+':'+ui->lineEdit->text());
        ui->lineEdit->clear();
    }
}

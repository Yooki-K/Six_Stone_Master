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
        ui->widget->move(WIDTH-ui->widget->width()-200,HEIGHT/2-ui->widget->height()/2);
    }
    ui->btback->hide();
    ui->btconc->hide();
    ui->btgv->hide();
    ui->setvolume->hide();
    ui->volume->hide();
    if(game->isonline==0)
    setMouseTracking(true);
    ui->btmyset->installEventFilter(this);  //在窗体上为btmyset安装过滤器
    ui->setvolume->installEventFilter(this);
    Sound_effect = new QMediaPlayer(this);
    Sound_effect->setMedia(QUrl("qrc:/new/myresouce/reso/music/down.wav"));
    Sound_effect->setVolume(100);
    if(game->isonline!=-1)
    {
        ui->flagchoose->close();
        ui->player2time->close();
        ui->p1->setScaledContents(true);
        ui->p2->setScaledContents(true);
        connect(game,SIGNAL(change(bool)),this,SLOT(change(bool)));
        ui->btwhite->setStyleSheet("background-color:rgb(0,0,0,120)");
    }
    else{
        ui->meslist->close();
        ui->btsend->close();
        ui->lineEdit->close();
        ui->name_1->close();
        ui->name_2->close();
        ui->tx_1->close();
        ui->tx_2->close();
    }
    timeID=startTimer(1000);
    ui->tx_1->setScaledContents(true);
    ui->tx_2->setScaledContents(true);
    ui->name_1->setReadOnly(true);
    ui->name_2->setReadOnly(true);
}


Chessboard::~Chessboard()
{
    delete ui;
}

void Chessboard::closeEvent(QCloseEvent *e)
{
    QMessageBox::information(NULL,"游戏尚在进行中","如果需要退出游戏，请认输");
    e->ignore();
}

void Chessboard::setbegin()
{
    if(game->player1->myflag)
    {
        ui->p1->setPixmap(QPixmap(":/new/myresouce/reso/pix/black.png"));
        ui->p2->setPixmap(QPixmap(":/new/myresouce/reso/pix/white.png"));
        ui->p2->hide();
    }
    else{
        ui->p2->setPixmap(QPixmap(":/new/myresouce/reso/pix/black.png"));
        ui->p1->setPixmap(QPixmap(":/new/myresouce/reso/pix/white.png"));
        ui->p1->hide();
    }
}
void Chessboard::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(QPen(QColor("black")));
    painter.setRenderHint(QPainter:: Antialiasing, true);  //设置渲染,启动反锯齿
    painter.setBrush(QColor(230,255,255,135));
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
    isselected=0;
    if(  game->state!=playing) return;
    if(  game->type==AA)return;
    if(  game->type==MA&&game->Gameflags!=game->player1->myflag) return;
    if(parent()==0)
    {
        x=event->x()-200;
        y=event->y()-50;
    }
    else{
        x=event->x();
        y=event->y();
    }
    if(x<margin||y<margin||x>margin+20*one||y>margin+20*one)return; 
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

void Chessboard::timerEvent(QTimerEvent *event)
{
    if(game->player1==0) return;
    if(event->timerId()==timeID)
    {
        QString t,tt;
        if(game->isonline==0)
        {

            if(game->player1->ontime->remainingTime()>0)
                t=game->player1->inttotime(game->player1->ontime->remainingTime());
            else
                t=game->player1->inttotime(game->player1->ontime->interval());
            ui->player1time->display(t);
        }
        else{
            if(game->player2==0) return;
            int ttime1=game->player1->ontime->remainingTime();
            int ttime2=game->player2->ontime->remainingTime();
            if(ttime1==0||ttime2==0) return;
            if(ttime1>0)
                t=game->player1->inttotime(ttime1);
            else
                t=game->player1->inttotime(game->player1->ontime->interval());
            ui->player1time->display(t);
            if(ttime2>0)
                tt=game->player2->inttotime(ttime2);
            else
                tt=game->player2->inttotime(game->player2->ontime->interval());
            ui->player2time->display(tt);
        }
    }
}

bool Chessboard::eventFilter(QObject *watched, QEvent *event)
{
   if (watched==ui->btmyset)
   {
        if (event->type()==QEvent::Enter)     //然后再判断控件的具体事件 (这里指鼠标进入事件)
        {
          ui->btmyset->setStyleSheet("background-image: url(:/new/myresouce/reso/button/set_1.png);background-color:transparent;");
          if(ui->flagchoose->isHidden())
              ui->btmyset->setEnabled(1);
          else
              ui->btmyset->setEnabled(0);
        }
        if (event->type()==QEvent::Leave)    // 这里指鼠标离开事件
        {
           ui->btmyset->setStyleSheet("background-image: url(:/new/myresouce/reso/button/set_2.png);background-color:transparent;");
         }

   }
   if(watched==ui->setvolume){
       if(event->type()==QEvent::Enter){
           ui->volume->show();
       }
       else if(event->type()==QEvent::Leave)
       {
           ui->volume->hide();
       }
   }
   return 0;
}

void Chessboard::setmes(QPixmap p1, QString n1, QPixmap p2, QString n2)
{
    ui->name_1->setText(n1);
    ui->name_2->setText(n2);
    ui->tx_1->setPixmap(p1);
    ui->tx_2->setPixmap(p2);
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
    if(game->isonline==-1)
    {
        if(game->Gameflags==game->player1->myflag)
            game->backStep(game->player1);
        else
            game->backStep(game->player2);
    }
    else{
        emit game->sendback();
    }
}


void Chessboard::on_btgv_clicked()
{
    if(game->isonline==-1)
    {
        if(game->Gameflags==game->player1->myflag)
            game->giveup(game->player1);
        else
            game->giveup(game->player2);
    }
    else{
        emit game->sendgv();
    }

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


void Chessboard::on_btmyset_clicked()
{
    if(ui->btback->isHidden())
    {
        ui->btback->show();
        ui->btconc->show();
        ui->btgv->show();
    }
    else{
        ui->btback->hide();
        ui->btconc->hide();
        ui->btgv->hide();
        if(!ui->setvolume->isHidden())
            ui->setvolume->hide();
    }
}


void Chessboard::on_setvolume_valueChanged(int value)
{
    ui->volume->setText("音量："+QString::number(value));
    Sound_effect->setVolume(value);
}

void Chessboard::on_btconc_clicked()
{
    if(ui->setvolume->isHidden())
        ui->setvolume->show();
    else
        ui->setvolume->hide();
}

void Chessboard::change(bool p)
{
    if(p){
        ui->p1->show();
        ui->p2->hide();
    }
    else{
        ui->p2->show();
        ui->p1->hide();
    }
}

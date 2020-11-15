#include "chessboard.h"
#include "ui_chessboard.h"
#include"gplayer.h"
//#include<QDebug>
Chessboard::Chessboard(QWidget *parent, Gamemodel *game) ://paretn为mainwindow
    QWidget(parent),game(game),
    ui(new Ui::Chessboard)
{
    move(200,50);
    ui->setupUi(this);
    setMouseTracking(true);
    Sound_effect = new QMediaPlayer(this);
    Sound_effect->setMedia(QUrl("qrc:/reso/music/down.wav"));//设置落子音效
    Sound_effect->setVolume(100);
    timeID=startTimer(1000);
    ui->btmyset->installEventFilter(this);  //在窗体上为btmyset安装过滤器
    ui->setvolume->installEventFilter(this);
    ui->btback->hide();
    ui->btconc->hide();
    ui->btgvup->hide();
    ui->setvolume->hide();
    ui->volume->hide();
    ui->tx_1->setScaledContents(true);
    ui->tx_2->setScaledContents(true);
    ui->name_1->setReadOnly(true);
    ui->name_2->setReadOnly(true);
    ui->p1->setScaledContents(true);
    ui->p2->setScaledContents(true);
    QPixmap h(":/reso/pix/hand.png");
    hand.operator =(QCursor(h,h.width()/2,h.height()/2));
    //当前设置下棋方
    if(game->player1->myflag)
    {
        ui->p1->setPixmap(QPixmap(":/reso/pix/black.png"));
        ui->p2->setPixmap(QPixmap(":/reso/pix/white.png"));
        ui->p2->hide();
    }
    else{
        ui->p2->setPixmap(QPixmap(":/reso/pix/black.png"));
        ui->p1->setPixmap(QPixmap(":/reso/pix/white.png"));
        ui->p1->hide();
    }
    connect(game,SIGNAL(change(bool)),this,SLOT(change(bool)));
}


Chessboard::~Chessboard()
{
    delete ui;
}
void Chessboard::setmes(QPixmap p1, QString n1, QPixmap p2, QString n2)
{
    ui->name_1->setText(n1);
    ui->name_2->setText(n2);
    ui->tx_1->setPixmap(p1);
    ui->tx_2->setPixmap(p2);
}

void Chessboard::on_setvolume_valueChanged(int value)
{
    ui->volume->setText("音量："+QString::number(value));
    Sound_effect->setVolume(value);
}
void Chessboard::on_btconc_clicked()
{
    if(ui->setvolume->isHidden())
    {
        ui->setvolume->show();
    }
    else
    {
        ui->setvolume->hide();
    }
}
void Chessboard::timerEvent(QTimerEvent *event)//显示玩家计时器，需要在20分钟内完成游戏
{
    if(game->player1==0) return;
    if(event->timerId()==timeID)
    {
        QString t;
        if(game->player1->ontime->remainingTime()==0)return;
        if(game->player1->ontime->remainingTime()>0)
            t=game->player1->inttotime(game->player1->ontime->remainingTime());
        else
            t=game->player1->inttotime(game->player1->ontime->interval());
        if(game->player1->istimeover) {ui->player1time->display("00:00");return;}
        ui->player1time->display(t);
    }
}
void Chessboard::paintEvent(QPaintEvent *)//绘制棋盘，棋子等信息
{
    QPainter painter(this);
    painter.setPen(QPen(QColor("black")));
    painter.setRenderHint(QPainter:: Antialiasing, true);  //设置渲染,启动反锯齿
    painter.setBrush(QColor(230,255,255,135));
    painter.drawRect(margin-2*r,margin-2*r,one*20+4*r,one*20+4*r);//画轮廓
    for(int i=0;i<rowline;i++){
        painter.drawLine(margin,margin+i*one,margin+20*one,margin+i*one);
    }
    for(int i=0;i<columnline;i++){
        painter.drawLine(margin+i*one,margin,margin+i*one,margin+20*one);
    }//画棋牌
    painter.setPen(QPen(QColor("blue")));
    if(isselected){
        if( game->Gameflags){
            painter.setBrush(QColor("black"));
        }else{
            painter.setBrush(QColor("white"));
        }
        QPoint ing(clickx*one+margin,clicky*one+margin);
        painter.drawEllipse(ing,r,r);
    }//鼠标监听，鼠标为落子时的坐标显示
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
    if(  game->state==win){
        if(game->winx<0||game->winy<0) return;
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

void Chessboard::mouseMoveEvent(QMouseEvent *event)//鼠标监听，当玩家为下棋方，鼠标移动到棋盘上会出现棋子影像
{
    isselected=0;setCursor(Qt::ArrowCursor);
    if(  game->state!=playing) return;
    if(  game->type==AA)return;
    if(  game->type==MA&&game->Gameflags!=game->player1->myflag) return;
    x=event->x();
    y=event->y();
    if(x<margin||y<margin||x>margin+20*one||y>margin+20*one)return;
    this->setCursor(hand);
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
    if(clickx<0||clicky<0||clickx>20||clicky>20) return;//控制范围，防止程序异常
        if(  game->game_progress[clickx][clicky]==isempty){
                isselected=1;
                update();
        }
    }


}

void Chessboard::mouseReleaseEvent(QMouseEvent *)//落子
{

    if(  game->state!=playing) return;
    if(  game->type==AA) return;//AA模式
    if(  game->type==MA&&game->Gameflags!=game->player1->myflag) return;//AI下棋时不监听
    if(isselected){
        emit mouseRelease();
    }
}

void Chessboard::on_btback_clicked()
{
    emit sendback();
}


void Chessboard::on_btgvup_clicked()
{
    emit sendgiveup();
}

void Chessboard::receivemeschat(QString mes){//获得玩家聊天信息
    ui->meslist->addItem(mes);
}

void Chessboard::on_btsend_clicked()//发送玩家聊天信息
{
    if(!ui->lineEdit->text().isEmpty())
    {
        emit sendmesschat(QTime::currentTime().toString()+' '+game->player1->name+':'+ui->lineEdit->text());
        ui->meslist->addItem(QTime::currentTime().toString()+' '+game->player1->name+':'+ui->lineEdit->text());
        ui->lineEdit->clear();
    }
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
bool Chessboard::eventFilter(QObject *watched, QEvent *event)
{
   if (watched==ui->btmyset)
   {
        if (event->type()==QEvent::Enter)     //然后再判断控件的具体事件 (这里指鼠标进入事件)
        {
          ui->btmyset->setStyleSheet("background-image: url(:/reso/button/set_1.png);background-color:transparent;");
        }
        if (event->type()==QEvent::Leave)    // 这里指鼠标离开事件
        {
           ui->btmyset->setStyleSheet("background-image: url(:/reso/button/set_2.png);background-color:transparent;");
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
   return QWidget::eventFilter(watched,event);
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

void Chessboard::on_btmyset_clicked()
{

        if(ui->btback->isHidden())
        {
            ui->btback->show();
            ui->btconc->show();
            ui->btgvup->show();
        }
        else{
            ui->btback->hide();
            ui->btconc->hide();
            ui->btgvup->hide();
            if(!ui->setvolume->isHidden())
                ui->setvolume->hide();
        }
}


void Chessboard::on_ai_clicked()
{

   nofai++;
   if(nofai%2==0){//关闭
       game->isdeposit=0;
       ui->ai->setToolTip("开启托管");
       ui->ai->setStyleSheet("background-image: url(:/reso/pix/aiopen.png);background-color:transparent;");
       emit sendmesschat(QTime::currentTime().toString()+' '+game->player1->name+"关闭托管");
       ui->meslist->addItem(QTime::currentTime().toString()+' '+game->player1->name+"关闭托管");


   }else{//开启
       game->isdeposit=1;
       ui->ai->setToolTip("关闭托管");
       ui->ai->setStyleSheet("background-image: url(:/reso/pix/aiclose.png);background-color:transparent;");
       emit sendmesschat(QTime::currentTime().toString()+' '+game->player1->name+"开启托管");
       ui->meslist->addItem(QTime::currentTime().toString()+' '+game->player1->name+"关闭托管");

   }
   game->aifirst=1;
}

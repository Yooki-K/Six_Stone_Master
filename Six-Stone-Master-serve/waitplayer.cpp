#include "waitplayer.h"

Waitplayer::Waitplayer(int index, QPixmap p1, QString name1, QWidget *parent) : QDialog(parent),index(index)
{
    if(index==0)
    {
        player1n=new QLineEdit(name1);
        player1p=new QLabel();
        player2n=new QLineEdit("");
        player2p=new QLabel();
        player1n->setAlignment(Qt::AlignCenter);
        player2n->setAlignment(Qt::AlignCenter);
        player1p->setPixmap(p1);
        player2p->setStyleSheet("background-color:white");
        player1p->setScaledContents(true);
        player2p->setScaledContents(true);
        player1n->setFixedSize(100,25);
        player2n->setFixedSize(100,25);
        player1p->setFixedSize(100,130);
        player2p->setFixedSize(100,130);
        QVBoxLayout *vlayout1=new QVBoxLayout;
        vlayout1->addWidget(player1p);
        vlayout1->addWidget(player1n);
        QVBoxLayout *vlayout2=new QVBoxLayout;
        vlayout2->addWidget(player2p);
        vlayout2->addWidget(player2n);
        pk=new QLabel("PK");
        pk->setStyleSheet("font:50px;color:rgb(255,0,0);");
        QVBoxLayout *vlayout3=new QVBoxLayout;
        vlayout3->addWidget(pk);
        QHBoxLayout *all=new QHBoxLayout;
        all->addLayout(vlayout1);
        all->addLayout(vlayout3);
        all->addLayout(vlayout2);
        QHBoxLayout *waitit=new QHBoxLayout;
        wait_=new QLabel;
        wait_->setText(w);
        wait_->setFixedSize(200+pk->sizeHint().width(),25);
        wait_->setAlignment(Qt::AlignCenter);
        waitit->addWidget(wait_);
        QVBoxLayout *isall=new QVBoxLayout;
        isall->addLayout(all);
        isall->addLayout(waitit);
        this->setLayout(isall);
        this->setFixedSize(this->sizeHint());
        t=startTimer(800);
        player1n->setReadOnly(true);
        player2n->setReadOnly(true);
        this->setWindowTitle("等待玩家加入");
    }
    else{

    }
}
Waitplayer::Waitplayer(int index, QWidget *parent) : QDialog(parent),index(index)
{
    p=new QProgressBar;
    p->setFixedSize(300,25);
    QVBoxLayout *v=new QVBoxLayout;
    v->addWidget(p);
    this->setLayout(v);
    this->setFixedSize(this->sizeHint());
    tt=startTimer(100);
    p->setValue(0);
    p->setMinimum(0);
    p->setMaximum(100);
    this->setWindowTitle("正在加载游戏");
}

void Waitplayer::closeEvent(QCloseEvent *event)
{
    if(index==0)
        emit closeroom();
    if(index!=0){
        event->ignore();
    }
}

void Waitplayer::timerEvent(QTimerEvent *event)
{
    if(event->timerId()==t){
        if(n<5){
            ww.append(".");
            n++;
        }
        else{
            n=0;
            ww="";
        }
        wait_->setText(this->w+ww);
    }else if(event->timerId()==tt){
        int value=p->value();
        p->setValue(value+10);
    }
}

void Waitplayer::setplayer2(QPixmap p2, QString name2)
{

    player2p->setPixmap(p2);
    player2n->setText(name2);
    killTimer(t);
    wait_->setText("玩家："+name2+"加入房间，即将开始游戏");
    QEventLoop loop;
    QTimer::singleShot(1000,&loop,SLOT(quit()));
    loop.exec();

}

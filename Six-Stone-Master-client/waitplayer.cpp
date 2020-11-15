#include "waitplayer.h"

Waitplayer::Waitplayer(int index, QPixmap p1, QString name1, QWidget *parent) : QDialog(parent),index(index)
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
Waitplayer::Waitplayer(int index, QWidget *parent) : QDialog(parent),index(index){
    p=new QProgressBar;
    p->setFixedSize(300,25);
    QVBoxLayout *v=new QVBoxLayout;
    v->addWidget(p);
    this->setLayout(v);
    this->setFixedSize(this->sizeHint());
    t=startTimer(100);
    p->setValue(0);
    p->setMinimum(0);
    p->setMaximum(100);
    this->setWindowTitle("正在加载游戏");
}

Waitplayer::Waitplayer(QString content, int index,QWidget *parent):QDialog(parent),index(index)
{
    QLabel *con=new QLabel(this);
    QLabel *text=new QLabel(this);
    text->setText(content);
    text->setStyleSheet("font-size:18px;color:black");
    con->lower();
    text->adjustSize();
    QPixmap mesbox(":/reso/pix/messagebox.png");
    con->setFixedSize(int(double(text->size().width())/(23-9)*23),text->size().height()/4*11);
    con->setPixmap(mesbox);
    con->setScaledContents(true);
    con->move(0,0);
    int w=con->size().width()/23*6.7;
    int h=con->size().height()/11.0*3.2;
    text->move(w,h);
    this->setWindowFlags(Qt::FramelessWindowHint);//去掉标题栏
    this->setFixedSize(con->size());
    this->move(1920,1080/2-this->height()/2);
    t=startTimer(2000/(1920+this->width()/2));
    this->setMouseTracking(true);
}
void Waitplayer::closeEvent(QCloseEvent *event)
{
    if(index==0)
        emit closeroom();
    if(index==1){
        event->ignore();
    }
    if(index==2){
        deleteLater();
    }
}

void Waitplayer::timerEvent(QTimerEvent *event)
{
    if(event->timerId()==t){
        switch (index) {
        case 0:
            if(n<5){
                ww.append(".");
                n++;
            }
            else{
                n=0;
                ww="";
            }
            wait_->setText(this->w+ww);
            break;
        case 1:
        {
            int value=p->value();
            p->setValue(value+10);
        }
            break;
        case 2:
            if(this->geometry().x()+this->width()<=0) close();
            if(this->geometry().x()==1920/2-this->width()/2){
                killTimer(t);
                QEventLoop loop;
                QTimer::singleShot(2000,&loop,SLOT(quit()));
                loop.exec();
                t=startTimer(2000/(1920+this->width()/2));
            }
            this->move(this->geometry().x()-2,this->geometry().y());
            break;
        default:
            break;
        }
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

void Waitplayer::mousePressEvent(QMouseEvent *)
{
    if(index!=2) return;
    killTimer(t);
}

void Waitplayer::mouseReleaseEvent(QMouseEvent *)
{
    if(index!=2) return;
    t=startTimer(2000/(1920+this->width()/2));
}

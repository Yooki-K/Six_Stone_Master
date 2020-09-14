#include "mainwindow.h"
#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    f=new Selectform(this);
    f->move(WIDTH/2-f->width()/2,HEIGHT/2-f->height()/2);
    connect(f,SIGNAL(sendmes(Gametype,GameAI)),this,SLOT(receimes(Gametype,GameAI)));//将select界面与棋盘界面进行数据传递
    connect(f,SIGNAL(online()),this,SLOT(openline()));
   // ui->widget->setGeometry(0,0,margin+20*one+100,margin+20*one+100);
    ui->label->hide();
    ui->label->setText("服务器本地端口");
    ui->label->move(WIDTH/2-ui->label->width()/2,20);
    ui->label->setAlignment(Qt::AlignHCenter);
    //设置动态背景
    QLabel* myback = new QLabel(this);
    myback->setGeometry(0,0,WIDTH,HEIGHT);
    QMovie * move = new QMovie(":/new/background/reso/star1.gif");
    myback->setMovie(move);
    myback->setScaledContents(true);//大小自适应
    move->start();
    connect(move,&QMovie::finished,move,[=](){move->start();});//动态背景循环
    myback->lower();
}

MainWindow::~MainWindow()
{
    delete f;
    delete b;
    delete
    server;
    delete ui;
}

void MainWindow::receimes(Gametype t1, GameAI ai1)
{
    b=new Chessboard(this);//建立棋盘
    b->show();
    ui->label->show();
    b->game->type=t1;
    b->game->AItype=ai1;
}//////////////////////////////////////////////////////////////////

void MainWindow::openline()
{
    server=new Server(this);//建立服务器
    ui->label->show();
}

void MainWindow::updatelabel(QString dk)
{
    ui->label->setText("当前服务器端口:"+dk);
}

void MainWindow::GameOver(Gamestate state, bool flag)
{

    QString gameend;
    if(state==win) {
        if(!flag) gameend="白方";
        else     gameend="黑方";
        gameend+="获胜,是否重新开始游戏";
    }
    else gameend="双方和棋,是否重新开始游戏";
    QMessageBox:: StandardButton result=QMessageBox::information(NULL, "游戏结束", gameend,QMessageBox::Yes|QMessageBox::No);
    if(result==QMessageBox::Yes)//只是重新开始游戏，游戏模式不变
    {
        delete b;
        f->show();
     }
   else{
       exit(0);
    }

}




#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QLabel>
#include<QMovie>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    pDesktopWidget = QApplication::desktop();
    deskRect = pDesktopWidget->availableGeometry();//获取当前桌面
    f=new Selectform(this);
    f->setWindowTitle("六子棋服务器首页");
    f->move(deskRect.width()/2-f->width()/2,deskRect.height()/2-f->height()/2);
    connect(f,SIGNAL(sendmes(Gametype,GameAI)),this,SLOT(receimes(Gametype,GameAI)));//将select界面与棋盘界面进行数据传递
    connect(f,SIGNAL(online()),this,SLOT(openline()));

    ui->widget->setGeometry(0,0,margin+20*one+100,margin+20*one+100);
    ui->widget->hide();
    ui->label->hide();
    ui->label->setText("服务器本地端口");
    ui->label->move(deskRect.width()/2-ui->label->width()/2,20);
    ui->label->setAlignment(Qt::AlignHCenter);
    //设置动态背景
    QLabel* myback = new QLabel(this);
    myback->setGeometry(0,0,deskRect.width(),deskRect.height());
    QMovie * move = new QMovie(":/new/background/reso/star1.gif");
    myback->setMovie(move);
    myback->setScaledContents(true);//大小自适应
    move->start();
    connect(move,&QMovie::finished,move,[=](){move->start();});//动态背景循环
    myback->lower();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::receimes(Gametype t1, GameAI ai1)
{
    ui->widget->show();
    ui->label->show();
    ui->widget->game->type=t1;
    ui->widget->game->AItype=ai1;
}

void MainWindow::openline()
{
    server=new Server(this);
    ui->label->show();
}

void MainWindow::updatelabel(QString dk)
{
    ui->label->setText("当前服务器端口:"+dk);
}





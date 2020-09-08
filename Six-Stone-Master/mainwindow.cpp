#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QLabel>
#include<QMovie>
#include<QDesktopWidget>
#include<QRect>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->widget->setGeometry(0,0,margin+20*one+100,margin+20*one+100);
    //设置动态背景
    QDesktopWidget* pDesktopWidget = QApplication::desktop();
    QRect deskRect = pDesktopWidget->availableGeometry();//获取当前桌面
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




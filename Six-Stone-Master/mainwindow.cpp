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
    QDesktopWidget* pDesktopWidget = QApplication::desktop();
    QRect deskRect = pDesktopWidget->availableGeometry();//获取当前桌面
    ui->setupUi(this);
    ui->widget->setGeometry(0,0,margin+20*one+100,margin+20*one+100);
    ui->widget->hide();
    ui->selecttype->move(deskRect.width()/2-ui->selecttype->width()/2,deskRect.height()/2-ui->selecttype->height()/2);

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




void MainWindow::on_ButtonMM_clicked()
{
    ui->widget->show();
    ui->widget->game->type=MM;
    ui->selecttype->hide();
}

void MainWindow::on_ButtonMA_clicked()
{
    ui->widget->show();
    ui->widget->game->type=MA;
    ui->selecttype->hide();
}

void MainWindow::on_ButtonAA_clicked()
{
    ui->widget->show();
    ui->widget->game->type=AA;
    ui->selecttype->hide();
}

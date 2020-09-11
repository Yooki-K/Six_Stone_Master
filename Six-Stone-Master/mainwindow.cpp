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
    ui->selecttype->move(deskRect.width()/2-ui->selecttype->width()/2,deskRect.height()/2-ui->selecttype->height()/2);//将模式选择界面放中心
    //设置动态背景
    ui->ButtonAA->installEventFilter(this);  //在窗体上为ButtonAA安装过滤器
    ui->ButtonMA->installEventFilter(this);  //在窗体上为ButtonMA安装过滤器
    ui->ButtonMM->installEventFilter(this);  //在窗体上为ButtonMM安装过滤器
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
bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
     if (watched==ui->ButtonAA)
     {
          if (event->type()==QEvent::Enter)     //然后再判断控件的具体事件 (这里指鼠标进入事件)
          {
            ui->ButtonAA->setStyleSheet("color:rgb(255,255,255);font: 45pt;");
          }
          else if (event->type()==QEvent::Leave)    // 这里指鼠标离开事件
          {
             ui->ButtonAA->setStyleSheet("color:rgb(0,0,255);font: 30pt;");
           }
     }
     if (watched==ui->ButtonMA)
     {
          if (event->type()==QEvent::Enter)
          {
            ui->ButtonMA->setStyleSheet("color:rgb(255,255,255);font: 45pt;");
          }
          else if (event->type()==QEvent::Leave)
          {
             ui->ButtonMA->setStyleSheet("color:rgb(0,0,255);font: 30pt;");
           }
     }

     if (watched==ui->ButtonMM)
     {
          if (event->type()==QEvent::Enter)
          {
            ui->ButtonMM->setStyleSheet("color:rgb(255,255,255);font: 45pt;");
          }
          else if (event->type()==QEvent::Leave)
          {
             ui->ButtonMM->setStyleSheet("color:rgb(0,0,255);font: 30pt;");
           }
     }


 return QWidget::eventFilter(watched,event);     // 最后将事件交给上层对话框
}

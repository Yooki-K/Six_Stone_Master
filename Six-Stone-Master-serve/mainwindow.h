#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "gamemodel.h"
#include"chessboard.h"
#include"selectform.h"
#include"severform.h"
#include"server.h"
#include <QMainWindow>
#include<QLabel>
#include<QMovie>
#include<QPushButton>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    Selectform *f=nullptr;//选择界面指针
    Gamemodel *localgame=nullptr;//本地端游戏指针
    SeverForm *sf=nullptr;//服务器界面指针
    Server *server=nullptr;//服务器指针
    void closeEvent(QCloseEvent *event);
private slots:


private:
    Ui::MainWindow *ui;
public slots:
    void receimes(Gametype t1,GameAI ai1);//接收玩家选择的模式和难度
    void openline();//开启服务器槽函数
    void GameOver(int ,bool);//本地端口游戏结束
signals:
    void sendsetmes(QPixmap,QString,QPixmap,QString);//发送一个房间内两个玩家个人信息
};

#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"client.h"
#include<QListWidgetItem>
#include<QHostInfo>
#include<QMovie>
#include<QRegExp>
namespace Ui {
class MainWindow;
}
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    Client *client=0;//客户端指针
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void sendhelp(QString);//小助手提示函数
private slots:
    void receiveMessage(QByteArray);//接收来自服务器消息
    void on_btconnect_clicked();//连接服务器按钮点击事件
    void on_btdiscon_clicked();//断开服务器按钮点击事件    
    void on_playerroom_itemDoubleClicked(QListWidgetItem *item);//连接房间

    void on_btopen_clicked();//开房按钮点击事件
    void GameOver();//游戏结束
    void receiveprogress(QString);//接收棋盘信息
    void on_toolButton_clicked();//刷新界面
    void on_btupdatemes_clicked();//修改玩家个人信息按钮点击事件
    void showEvent(QShowEvent *);
private:
    Ui::MainWindow *ui;
    QString ip;//ip
    QMovie *yuxiaoyu=0;//助手gif
    QFile *permes=0;//个人信息存储文件，路径与可执行文件一致
    QPixmap *Pix=0;//头像图片
signals:
    void isokon();//可以下棋信号
    void receivemeschat(QString);//接收玩家聊天消息
    void sendsetmes(QPixmap,QString,QPixmap,QString);//发送同个房间两个玩家个人信息
};

#endif // MAINWINDOW_H

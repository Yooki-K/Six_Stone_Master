#ifndef SEVERFORM_H
#define SEVERFORM_H
#include<QListWidgetItem>
#include <QWidget>
#include"server.h"
#include"waitplayer.h"
#include<QFile>
#include<QMovie>
#include<QRegExp>
#include<QFileDialog>
namespace Ui {
class SeverForm;
}

class SeverForm : public QWidget
{
    Q_OBJECT
private:
    Ui::SeverForm *ui;
    QMovie *yuxiaoyu=0;//小助手的gif动图存放
    QFile *permes=0;//个人信息储存在此文件，与可执行程序同路径
    QPixmap *Pix=0;//头像图片存储
    Waitplayer *w=nullptr;
public:
    explicit SeverForm(QWidget *parent = 0);
    ~SeverForm();
    Server*server=0;//服务器类的指针

private slots:
    void on_btcloses_clicked();//关闭服务器按钮点击事件
    void updateroom(Server *);//更改房间函数
    void on_playerroom_itemDoubleClicked(QListWidgetItem *item);//双击连接房间函数
    void on_btreflash_clicked();//刷新界面函数
    void on_btupdatemes_clicked();//修改个人信息按钮点击事件
    void showEvent(QShowEvent*);

    void on_getpath_clicked();

public slots:
    void btopenclicked(QString ip="127.0.0.1", QString text="开房");//开房按钮点击事件
    void updatenum(int);//更改连接人数
    void updatemesbox(QString);//接收并显示客户端连接和断开信息
    void sendhelp(QString);//小助手提示函数

signals:
    void closesever();//关闭服务器信号
    void openroom(QString);//开房信号
    void closeroom();//关闭房间信号
    void connectroom(QString);//连接房间信号
    void sendsetmes(QPixmap,QString,QPixmap,QString);//发送一个房间两个玩家个人信息（用户名、头像）信号
};

#endif // SEVERFORM_H

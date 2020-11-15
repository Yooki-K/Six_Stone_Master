#ifndef CHESSBOARD_H
#define CHESSBOARD_H
#include "gamemodel.h"
#include<QMessageBox>
#include<QString>
#include<QWidget>
#include<QPainter>
#include<QKeyEvent>
#include<math.h>
#include<QTime>
#include<QMediaPlayer>
#include<QCursor>
namespace Ui {
class Chessboard;
}

class Chessboard : public QWidget
{
    Q_OBJECT
public:
    int nofai=0;
    bool isselected=0;//鼠标监听，当前位置是否可以落子
    int x=-1;int y=-1;//鼠标坐标
    int clickx=-1;int clicky=-1;//落子坐标
    Gamemodel *game=0;//游戏进程指针
    int timeID;//计时器ID
    explicit Chessboard(QWidget *parent = 0,Gamemodel *game=0);
    ~Chessboard();
    QMediaPlayer *Sound_effect=0;//落子音效
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void timerEvent(QTimerEvent *event);  
    bool eventFilter(QObject *watched, QEvent *event);//事件过滤
private slots:
    void on_btconc_clicked();//调节音量按钮点击事件
    void on_btback_clicked();//悔棋
    void on_btgvup_clicked();//认输2
    void on_btsend_clicked();//发送消息按钮点击事件
    void on_lineEdit_returnPressed();//消息框回车发送消息
    void on_btmyset_clicked();//设置按钮
    void on_setvolume_valueChanged(int value);//音量改变时调用的函数
    void on_ai_clicked();

signals:
    void mouseRelease();//落子信号
    void sendmesschat(QString);//发送玩家聊天消息
    void sendback();//悔棋信号
    void sendgiveup();//认输信号
public slots:
    void receivemeschat(QString);//接收玩家聊天消息
    void setmes(QPixmap,QString,QPixmap,QString);//接收两个玩家个人信息
    void change(bool);//更改下棋方
private:
    Ui::Chessboard *ui;
    QCursor hand;
};

#endif // CHESSBOARD_H

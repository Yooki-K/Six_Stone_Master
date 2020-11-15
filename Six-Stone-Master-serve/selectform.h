#ifndef SELECTFORM_H
#define SELECTFORM_H
#include <QWidget>
#include"gamemodel.h"
namespace Ui {
class Selectform;
}

class Selectform : public QWidget
{
    Q_OBJECT

public:
    explicit Selectform(QWidget *parent = 0);
    ~Selectform();
    void ButtonMMclicked();//选择人人模式按钮点击事件
    void ButtonMAclicked();//选择人机模式按钮点击事件

    void ButtonAAclicked();//选择机机模式按钮点击事件
    bool eventFilter(QObject *, QEvent *);

    void ButtonEasyclicked();//机器人选择简单模式按钮点击事件

    void ButtonDiffclicked();//机器人选择困难模式按钮点击事件

    void ButtonOnclicked();//开启服务器按钮点击事件

private:
    Ui::Selectform *ui;
signals:
    void sendmes(Gametype,GameAI);//发送玩家选择模式和机器人难度信号
    void online();//开启服务器信号
private slots:
    void on_ButtonBack_clicked();
};

#endif // SELECTFORM_H

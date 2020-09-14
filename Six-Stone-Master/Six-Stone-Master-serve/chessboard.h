#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include<QMessageBox>
#include<QString>
#include<QWidget>
#include<QPainter>
#include<QKeyEvent>
#include<math.h>
#include<ai.h>
#include<QMediaPlayer>
#define WIDTH 1920
#define HEIGHT 1080
namespace Ui {
class Chessboard;
}

class Chessboard : public QWidget
{
    Q_OBJECT
    bool isselected=0;
    int x=-1.0;
    int y=-1.0;
    int clickx=-1;
    int clicky=-1;
    int backx=-1;
    int backy=-1;
public:
    explicit Chessboard(QWidget *parent = 0);
    ~Chessboard();
    QMediaPlayer *Sound_effect;
    Gamemodel * game;
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void backStep();
    void changeto(char *);
    char *changefrom();
    void startgame();
private slots:
    void on_btwhite_clicked();
    void on_btblack_clicked();
signals:
    void mouseRelease();
private:
    Ui::Chessboard *ui;
};

#endif // CHESSBOARD_H

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
    bool isselected=0;
    int x=-1.0;
    int y=-1.0;
    int nofai=0;
    int clickx=-1;
    int clicky=-1;
    Gamemodel *game=0;
    int timeID;

    explicit Chessboard(QWidget *parent = 0,Gamemodel *game=0);
    ~Chessboard();
    QMediaPlayer *Sound_effect;
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void closeflag();
    void timerEvent(QTimerEvent *);
    bool eventFilter(QObject *, QEvent *);
    void closeEvent(QCloseEvent *);
    void setbegin();
private slots:
    void on_btwhite_clicked();
    void on_btblack_clicked();
    void on_btback_clicked();
    void on_btgv_clicked();
    void on_btsend_clicked();
    void updatechat(QString);
    void on_lineEdit_returnPressed();
    void on_btmyset_clicked();
    void on_setvolume_valueChanged(int value);
    void on_btconc_clicked();
    void change(bool);
    void on_ai_clicked();

public slots:
    void setmes(QPixmap,QString,QPixmap,QString);
signals:
    void mouseRelease();
    void sendmesschat(QString);

private:
    Ui::Chessboard *ui;
    QCursor hand;
};

#endif // CHESSBOARD_H

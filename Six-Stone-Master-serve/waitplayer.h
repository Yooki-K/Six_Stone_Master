#ifndef WAITPLAYER_H
#define WAITPLAYER_H

#include <QDialog>
#include<QPixmap>
#include<QLabel>
#include<QBoxLayout>
#include <QLineEdit>
#include<QEventLoop>
#include<QTimerEvent>
#include<QTimer>
#include<QProgressBar>
#include<QCloseEvent>
class Waitplayer : public QDialog
{
    Q_OBJECT
private:
//    QPixmap p1;
    QPixmap p2;
//    QString name1;
    QString name2;
    int t;
    int tt;
    QString w="正在等待玩家加入房间";
    QString ww;
    int n;
    int index;
    QProgressBar *p;
    QLineEdit *player1n;
    QLabel *player1p;
    QLineEdit *player2n;
    QLabel *player2p;
    QLabel *pk;
    QLabel *wait_;
public:
    explicit Waitplayer(int index,QPixmap p1, QString name1, QWidget *parent = nullptr);
    explicit Waitplayer(int index=1, QWidget *parent = nullptr);
    void closeEvent(QCloseEvent *);
    void timerEvent(QTimerEvent *event);
signals:
    void closeroom();
public slots:
    void setplayer2(QPixmap,QString);
};

#endif // WAITPLAYER_H

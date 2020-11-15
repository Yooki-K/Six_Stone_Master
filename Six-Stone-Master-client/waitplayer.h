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
#include<QDebug>
class Waitplayer : public QDialog
{
    Q_OBJECT
private:
    QPixmap p2;
    QString name2;
    int t;
    double o=1.0;
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
    explicit Waitplayer(QString content, int index=2,QWidget *parent = nullptr);
    void closeEvent(QCloseEvent *);
    void timerEvent(QTimerEvent *event);
    void setplayer2(QPixmap,QString);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
signals:
    void closeroom();
public slots:
};

#endif // WAITPLAYER_H

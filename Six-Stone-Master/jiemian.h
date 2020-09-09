#ifndef JIEMIAN_H
#define JIEMIAN_H

#include<QWidget>
#include<QPainter>
#include<QKeyEvent>
#include<math.h>
#include<QtAlgorithms>
#include<gamemodel.h>
#include<QMediaPlayer>

class Jiemian : public QWidget
{
    Q_OBJECT

    bool isselected=0;
    int x=-1.0;
    int y=-1.0;
    int clickx=-1;
    int clicky=-1;
public:
    QMediaPlayer *Sound_effect;
    Gamemodel * game;
    explicit Jiemian(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    ~Jiemian();
signals:

public slots:

};

#endif // JIEMIAN_H

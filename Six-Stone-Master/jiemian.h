#ifndef JIEMIAN_H
#define JIEMIAN_H

#include<QWidget>
#include<QPainter>
#include<QKeyEvent>
#include<math.h>
#include<QtAlgorithms>
#include<vector>
#include<gamemodel.h>
const int margin=45;
const int rowline=21;
const int columnline=21;
const int r=20;
const int one=45;
enum what{isempty=-1,iswhite,isblack};

class Jiemian : public QWidget
{
    Q_OBJECT
    Gamemodel * game;
    bool isselected=0;
    int x=-1.0;
    int y=-1.0;
    int clickx=-1;
    int clicky=-1;
public:
    std::vector<std::vector<what>> game_progress;
    explicit Jiemian(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *);
    ~Jiemian();
signals:

public slots:

};

#endif // JIEMIAN_H

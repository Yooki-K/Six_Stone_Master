#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "gamemodel.h"
#include"chessboard.h"
#include"selectform.h"
#include"severform.h"
#include"server.h"
#include <QMainWindow>
#include<QLabel>
#include<QMovie>
#include<QPushButton>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    Selectform *f=nullptr;
    Gamemodel *localgame=nullptr;
    SeverForm *sf=nullptr;
    Server *server=nullptr;
private slots:


private:
    Ui::MainWindow *ui;
    bool isserveropenroom=0;
public slots:
    void receimes(Gametype t1,GameAI ai1);
    void openline();
    void updatelabel(QString);
    void GameOver(int, bool);
    void GameOver();
    void receiveprogress(QString);
signals:
    void isokon();
};

#endif // MAINWINDOW_H

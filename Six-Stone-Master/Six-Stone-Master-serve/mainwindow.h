#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "chessboard.h"
#include"selectform.h"
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
    Selectform *f;
    Chessboard *b;
    Server *server;
private slots:

private:
    Ui::MainWindow *ui;
public slots:
    void receimes(Gametype t1,GameAI ai1);
    void openline();
    void updatelabel(QString);
    void GameOver(Gamestate,bool);
};

#endif // MAINWINDOW_H

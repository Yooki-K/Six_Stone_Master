#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include"jiemian.h"
#include <QMainWindow>
#include "console.h"
#include<QDesktopWidget>
#include<QRect>
#include<QString>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QDesktopWidget* pDesktopWidget ;
    QRect deskRect;
private slots:
//    void on_pushButton_clicked();

//   void on_widget_windowTitleChanged(const QString &title);

    void GameOver();
    void on_ButtonMM_clicked();

    void on_ButtonMA_clicked();

    void on_ButtonAA_clicked();
    bool eventFilter(QObject *, QEvent *);

    void on_ButtonEasy_clicked();

    void on_ButtonDiff_clicked();

    void on_ButtonAA_2_clicked();

    void on_ButtonOn_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

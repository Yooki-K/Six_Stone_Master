#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include"jiemian.h"
#include <QMainWindow>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
//    void on_pushButton_clicked();

//    void on_widget_windowTitleChanged(const QString &title);

    void on_ButtonMM_clicked();

    void on_ButtonMA_clicked();

    void on_ButtonAA_clicked();
    bool eventFilter(QObject *, QEvent *);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

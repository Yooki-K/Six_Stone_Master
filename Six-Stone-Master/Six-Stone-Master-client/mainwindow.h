#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"client.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    Client *client;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btconnect_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

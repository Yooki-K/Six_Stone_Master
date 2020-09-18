#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"client.h"
#include<QListWidgetItem>
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
    void receiveMessage();
    void receiveprogress(QString);
    void on_btconnect_clicked();

    void on_pushButton_clicked();

    void on_playerroom_doubleClicked(const QModelIndex &index);

    void on_btdiscon_clicked();

    void on_btopen_clicked();

    void on_playerroom_itemDoubleClicked(QListWidgetItem *item);

private:
    Ui::MainWindow *ui;
signals:
    void isokon();
    void receivemeschat(QString);
};

#endif // MAINWINDOW_H

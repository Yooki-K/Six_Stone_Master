#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"client.h"
#include<QListWidgetItem>
#include<QHostInfo>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    Client *client=0;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void receiveMessage(QByteArray);
    void receiveprogress(QString);
    void on_btconnect_clicked();

    void on_btdiscon_clicked();

    void on_btopen_clicked();

    void on_playerroom_itemDoubleClicked(QListWidgetItem *item);

    void GameOver();
private:
    Ui::MainWindow *ui;
    QString ip;
signals:
    void isokon();
    void receivemeschat(QString);
};

#endif // MAINWINDOW_H

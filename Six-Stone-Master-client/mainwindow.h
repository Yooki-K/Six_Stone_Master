#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"client.h"
#include<QListWidgetItem>
#include<QHostInfo>
#include<QMovie>
#include<QRegExp>
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
    void sendhelp(QString);
private slots:
    void receiveMessage(QByteArray);
    void receiveprogress(QString);
    void on_btconnect_clicked();

    void on_btdiscon_clicked();

    void on_btopen_clicked();

    void on_playerroom_itemDoubleClicked(QListWidgetItem *item);

    void GameOver();
    void on_toolButton_clicked();

    void on_btupdatemes_clicked();
    void showEvent(QShowEvent *);
private:
    Ui::MainWindow *ui;
    QString ip;
    QMovie *yuxiaoyu=0;
    QFile *permes=0;
    QPixmap *Pix=0;
signals:
    void isokon();
    void receivemeschat(QString);
    void sendsetmes(QPixmap,QString,QPixmap,QString);
};

#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include"jiemian.h"
#include"selectform.h"
#include"server.h"
#include <QMainWindow>
#include<QDesktopWidget>
#include<QRect>
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
    Server *server;
    QDesktopWidget* pDesktopWidget ;
    QRect deskRect;
private slots:
//    void on_pushButton_clicked();

//   void on_widget_windowTitleChanged(const QString &title);


private:
    Ui::MainWindow *ui;
public slots:
    void receimes(Gametype t1,GameAI ai1);
    void openline();
    void updatelabel(QString);
};

#endif // MAINWINDOW_H

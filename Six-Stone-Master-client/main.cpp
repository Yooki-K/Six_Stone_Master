#include "mainwindow.h"
#include <QApplication>
#include <QCoreApplication>
//获取程序当前运行目录

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    Client::apppath = a.QCoreApplication::applicationDirPath();
    w.showMaximized();
    w.setWindowTitle("六子棋客户端");
    w.show();
    return a.exec();
}

#include "mainwindow.h"
#include <QApplication>
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowFlags(w.windowFlags()& Qt::WindowMaximizeButtonHint& Qt::WindowMinimizeButtonHint);
    w.showMaximized();
    w.setWindowTitle("六子棋服务器");
    Server::apppath = a.QCoreApplication::applicationDirPath();//获取程序当前运行目录
    w.show();
    return a.exec();
}

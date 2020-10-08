#include "mainwindow.h"
#include <QApplication>
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    Client::apppath = a.QCoreApplication::applicationDirPath();//获得可执行程序路径
    w.showMaximized();
    w.setWindowTitle("六子棋客户端");
    w.show();
    return a.exec();
}

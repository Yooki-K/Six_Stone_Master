#include "mainwindow.h"
#include <QApplication>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowFlags(w.windowFlags()& Qt::WindowMaximizeButtonHint& Qt::WindowMinimizeButtonHint);
    w.showMaximized();
    w.setWindowTitle("六子棋");
    w.show();
    return a.exec();
}

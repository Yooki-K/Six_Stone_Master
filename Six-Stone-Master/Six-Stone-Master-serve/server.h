#ifndef SERVER_H
#define SERVER_H
#include<QApplication>
#include<QMessageBox>
#include <QObject>
#include<QDebug>
#include<QTcpServer>
#include<QTcpSocket>
#include <QtGlobal>//包含随机函数
#include<QTime>
#include<QByteArray>
#include<QClipboard>//对剪贴板进行操作
const int maxcurcon=4;
class Server : public QTcpServer
{
    Q_OBJECT
private:
    QTcpSocket *socket;
public:
    explicit Server(QObject *parent = nullptr);
    quint16 serverdk;
    int curconnum=0;
    char inc[1024];
    char toc[1024];

signals:
    void sendmess(QString);
private slots:
   void incomingConnection(qintptr handle);
   void receiveMessage();
   void sendMessage(QString);
};

#endif // SERVER_H

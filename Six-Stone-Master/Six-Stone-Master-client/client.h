#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include<QTcpSocket>
#include<QByteArray>
#include<QDebug>
#include<QHostAddress>
class Client : public QObject
{
    Q_OBJECT
private:
    QTcpSocket *socket;
public:
    explicit Client(QString,QString,QObject *parent = nullptr);
    char inc[1024];
    char toc[1024];
signals:

private slots:
void receiveMessage();
void sendMessage(QString toc);
};


#endif // CLIENT_H

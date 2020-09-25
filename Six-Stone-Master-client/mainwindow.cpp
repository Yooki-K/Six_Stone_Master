#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    MOVETOCENTER(ui->stackedWidget);
    ui->stackedWidget->setCurrentIndex(0);
    QHostInfo info = QHostInfo::fromName(QHostInfo::localHostName());
    ip=info.addresses().last().toString();//获取本地ip
}

MainWindow::~MainWindow()
{
    delete ui;
    if(client!=0){
        delete client;
    }
}




void MainWindow::receiveMessage(QByteArray arr)
{
        QString ss=QString(arr.data());
        int i=ss.section("##",0,0).toInt();
        QString s=QString(arr.data()).section("##",1,1);
        switch (i) {
        case 0://连接服务器失败
            QMessageBox::information(NULL,"连接失败",s);
            ui->stackedWidget->setCurrentIndex(0);
            break;
        case 1://连接服务器成功
            QMessageBox::information(NULL,"连接成功",s);
            client->sendMessagetos(COMM_CLIENT_IP,ip);
            break;
        case 2://服务器关闭
            QMessageBox::information(NULL,"服务器关闭",s);
            delete client;
            client=0;
            ui->stackedWidget->show();
            ui->stackedWidget->setCurrentIndex(0);
            ui->imputdk->clear();
            break;
        case 3://服务器发送大厅信息
        {
            ui->playerroom->clear();
            int i=s.section("//",0,0).toInt();
            QStringList player;
            for(int j=1;j<=i;j++){
                player<<s.section("//",j,j);
            }
            ui->playerroom->addItems(player);
        }
            break;
        case 4://双方准备，服务器开始游戏
            ui->stackedWidget->hide();
            client->game=new Gamemodel(client);
            client->game->c=new Chessboard(this,client->game);
            client->game->c->show();

            client->game->player1=new GPlayer(client->myflag,client->game,client->game,ip);
            connect(this,SIGNAL(receivemeschat(QString)),client->game->c,SLOT(receivemeschat(QString)));
            connect(client->game->c,SIGNAL(sendmesschat(QString)),client,SLOT(sendMesschat(QString)));
            connect(client->game->c,&Chessboard::sendback,client,[&](){
                if(client->game->backx==-1&&client->game->backy==-1)
                    QMessageBox::information(NULL,"请求失败","当前情况不能请求悔棋");
                else
                    client->sendMessagetos(COMM_CLIENT_UNDO,"对方请求悔棋##"+QString::number(client->game->backx)+"##"+QString::number(client->game->backy));
            });
            connect(client->game->c,&Chessboard::sendgiveup,client,[&](){
                client->sendMessagetos(COMM_CLIENT_LOSE,"对方认输");
                client->game->stop();
                delete client->game;
                client->game=0;
                ui->stackedWidget->show();
                ui->stackedWidget->setCurrentIndex(1);
            });
            client->game->start();

            break;
        case 5://某方玩家胜利，服务器结束游戏
        {
            QString xy=QString(arr.data()).section("##",2,2);
            client->game->state=client->game->GameEnd(xy.section("//",0,0).toInt(),xy.section("//",1,1).toInt());
            QEventLoop loop;
            QTimer::singleShot(1500,&loop,SLOT(quit()));
            loop.exec();
            QMessageBox::information(NULL,"游戏结束",s);
            client->game->stop();
            delete client->game;
            client->game=0;
        }
            ui->stackedWidget->show();
            ui->stackedWidget->setCurrentIndex(1);
            break;
        case 8://玩家发来消息，对方退出游戏
            QMessageBox::information(NULL,"游戏结束",s);
            client->game->stop();
            delete client->game;
            client->game=0;
            ui->stackedWidget->show();
            ui->stackedWidget->setCurrentIndex(1);
            break;
        case 9://玩家游戏操作：落子
        {
            for(int i=0;i<rowline;i++){
                for(int j=0;j<columnline;j++){
                    client->game->game_progress[i][j]=((what)QString(s[i*rowline+j]).toInt());
                    client->game->c->update();
                }
            }
            QEventLoop loop;
            QTimer::singleShot(500,&loop,SLOT(quit()));
            loop.exec();
            emit isokon();
        }
            break;
        case 10://玩家游戏操作：悔棋
        {
            QMessageBox:: StandardButton result=QMessageBox::information(NULL,"悔棋",s,QMessageBox::Yes|QMessageBox::No);
            if(result==QMessageBox::Yes){
                client->sendMessagetos(COMM_CLIENT_UNDO_YES,"");
            }else{
                client->sendMessagetos(COMM_CLIENT_UNDO_NO,"");
            }
        }
            break;
        case 11://玩家游戏操作：悔棋回复yes
            QMessageBox::information(NULL,"请求成功",s);
            client->game->backx=-1;client->game->backy=-1;
            break;
        case 12://玩家游戏操作：悔棋回复no
            QMessageBox::information(NULL,"请求失败",s);
            break;
        case 13://玩家游戏操作：认输
            QMessageBox::information(NULL,"游戏结束",s);
            client->game->stop();
            delete client->game;
            client->game=0;
            ui->stackedWidget->show();
            ui->stackedWidget->setCurrentIndex(1);
            break;
        case 15://玩家游戏操作：发送聊天信息
            emit receivemeschat(s);
            break;
        default:
            break;
        }
}

void MainWindow::receiveprogress(QString progress)
{
    client->sendMessagetos(COMM_CLIENT_GAMEOP,progress);
}


void MainWindow::on_btconnect_clicked()
{
    client=new Client(ui->imputdk->text(),ui->imputip->text(),this);
    if(client->iscon)
    {
        ui->stackedWidget->setCurrentIndex(1);
        connect(this,&MainWindow::destroyed,client->socket,&QTcpSocket::disconnectFromHost);
    }
}


void MainWindow::on_btdiscon_clicked()
{
    client->socket->disconnectFromHost();
        delete client;
        client=0;
        ui->stackedWidget->setCurrentIndex(0);
        ui->imputdk->clear();
        ui->playerroom->clear();

}

void MainWindow::on_btopen_clicked()
{
    client->sendMessagetos(COMM_CLIENT_CREATE,ip+"##"+ui->btopen->text());
    if(ui->btopen->text()==QString("开房"))
    {
        client->myflag=1;
        ui->btopen->setText("关房");
    }
    else{
        client->myflag=0;
        ui->btopen->setText("开房");
    }
}

void MainWindow::on_playerroom_itemDoubleClicked(QListWidgetItem *item)
{
    client->sendMessagetos(COMM_CLIENT_JOIN,item->text().section(" ",0,0)+"##"+ip);
    client->myflag=0;
}

void MainWindow::GameOver()
{
    if(client->game->state==win)
        client->sendMessagetos(COMM_CLIENT_WIN,client->game->player1->name+"胜利"+"##"+QString::number(client->game->backx)+"//"+QString::number(client->game->backy));
    else
        client->sendMessagetos(COMM_CLIENT_WIN,"双方和棋##"+QString::number(client->game->backx)+"//"+QString::number(client->game->backy));
}

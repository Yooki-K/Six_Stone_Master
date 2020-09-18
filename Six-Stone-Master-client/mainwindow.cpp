#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tabWidget->move(WIDTH/2-ui->tabWidget->width()/2,HEIGHT/2-ui->tabWidget->height()/2);
    ui->tabWidget->setCurrentIndex(0);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btconnect_clicked()
{
    client=new Client(ui->imputdk->text(),ui->imputip->text(),this);
    ui->tabWidget->setCurrentIndex(1);
    connect(this,&MainWindow::destroyed,client->socket,&QTcpSocket::disconnectFromHost);
}


void MainWindow::receiveMessage()
{
    if(client->socket->bytesAvailable()>0){//client->socket的有效字节
//        client->socket->read(inc,client->socket->bytesAvailable());
        QByteArray arr=client->socket->readAll();
        int i=QString(arr.data()).section("##",0,0).toInt();
        QString s=QString(arr.data()).section("##",1,1);
        switch (i) {
        case 0://连接服务器失败
            QMessageBox::information(NULL,"连接失败",s);
            ui->tabWidget->setCurrentIndex(0);
            break;
        case 1://连接服务器成功
            QMessageBox::information(NULL,"连接成功",s);
            break;
        case 2://服务器关闭
            QMessageBox::information(NULL,"服务器关闭",s);
            client->deleteLater();
            ui->tabWidget->setCurrentIndex(0);
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
        {
            ui->tabWidget->close();
            client->game=new Gamemodel(client);
            client->game->c=new Chessboard(this);
            client->game->c->show();
            client->game->player1=new GPlayer(client->myflag,client->game,client->game);
            connect(this,SIGNAL(receivemeschat(QString)),client->game->c,SLOT(receivemeschat(QString)));
            connect(client->game->c,SIGNAL(sendmesschat(QString)),client,SLOT(sendMesschat(QString)));
            connect(client->game->c,&Chessboard::sendback,client,[&](){
                client->sendMessagetos(COMM_CLIENT_UNDO,"对方请求悔棋");
            });
            connect(client->game->c,&Chessboard::sendgiveup,client,[&](){
                client->sendMessagetos(COMM_CLIENT_LOSE,"对方认输");
            });
            client->game->start();
        }
            break;
        case 5://某方玩家胜利，服务器结束游戏
        {
            QMessageBox::information(NULL,"游戏结束",s);
            client->game->c->close();//关闭页面
            delete client->game->c;//释放棋牌
            client->game->deleteLater();//释放游戏游戏进程
            ui->tab_2->show();
        }
            break;
        case 8://玩家发来消息，对方退出游戏
        {
            QMessageBox::information(NULL,"游戏结束",s);
            client->game->c->close();//关闭页面
            delete client->game->c;//释放棋牌
            client->game->deleteLater();//释放游戏游戏进程
            ui->tab_2->show();
        }
            break;
        case 9://玩家游戏操作：落子
        {
            for(int i=0;i<rowline;i++){
                for(int j=0;j<columnline;j++){
                    client->game->game_progress[i][j]=((what)QString(s[i*rowline+j]).toInt());
                    client->game->c->update();
                }
            }
            emit isokon();
        }
            break;
        case 10://玩家游戏操作：悔棋
        {
            QMessageBox:: StandardButton result=QMessageBox::information(NULL,"悔棋",s,QMessageBox::Yes|QMessageBox::No);
            if(result=QMessageBox::Yes){
                client->sendMessagetos(COMM_CLIENT_UNDO_YES,"");
            }else{
                client->sendMessagetos(COMM_CLIENT_UNDO_NO,"");
            }
        }
            break;
        case 11://玩家游戏操作：悔棋回复yes
        {
            QMessageBox::information(NULL,"提示",s);
        }
            break;
        case 12://玩家游戏操作：悔棋回复no
        {
            QMessageBox::information(NULL,"提示",s);
        }
            break;
        case 13://玩家游戏操作：认输
        {
            QMessageBox::information(NULL,"游戏结束",s);
            client->game->c->close();//关闭页面
            delete client->game->c;//释放棋牌
            client->game->deleteLater();//释放游戏游戏进程
            ui->tab_2->show();
        }
            break;
        case 15://玩家游戏操作：发送聊天信息
        {
            emit receivemeschat(s);
        }
            break;
        default:
        {
        }
            break;
        }

    }

}

void MainWindow::receiveprogress(QString progress)
{
    client->sendMessagetos(COMM_CLIENT_GAMEOP,progress);
}

void MainWindow::on_btdiscon_clicked()
{
    client->socket->disconnectFromHost();
    client->deleteLater();
    ui->tabWidget->setCurrentIndex(0);
}

void MainWindow::on_btopen_clicked()
{
    client->sendMessagetos(COMM_CLIENT_CREATE,"");
    client->myflag=1;
}

void MainWindow::on_playerroom_itemDoubleClicked(QListWidgetItem *item)
{
    client->sendMessagetos(COMM_CLIENT_JOIN,item->text().section(" ",0,0));
    client->myflag=0;
}

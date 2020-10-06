﻿#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    MOVETOCENTER(ui->all);
    ui->imputdk->setFocus();
    ui->stackedWidget->setCurrentIndex(0);
    QHostInfo info = QHostInfo::fromName(QHostInfo::localHostName());
    ip=info.addresses().last().toString();//获取本地ip
    //设置动态背景
    QLabel* myback = new QLabel(this);
    myback->setGeometry(0,40,WIDTH,HEIGHT-80);
    QMovie * move = new QMovie(":/reso/gif/lion.gif",QByteArray(),this);
    myback->setMovie(move);
    myback->setScaledContents(true);//大小自适应
    move->start();
    connect(move,&QMovie::finished,move,[&](){move->start();});//动态背景循环
    myback->lower();
    ui->txpath->hide();
    ui->tx->setScaledContents(true);
    ui->name->setReadOnly(true);
    yuxiaoyu=new QMovie(":/reso/head-portrait/yuyan.gif",QByteArray(),this);
    ui->helper->setMovie(yuxiaoyu);
    ui->helper->setScaledContents(true);
    ui->tx_2->setScaledContents(true);
}

MainWindow::~MainWindow()
{
    delete ui;
    if(client!=0){
        delete client;
    }
}

void MainWindow::sendhelp(QString mes)
{
    yuxiaoyu->start();
    QTimer t;
    int n=0;
    int m=mes.toLocal8Bit().size();
    if(m==0) return;
    int p=m/26;
    int i=1;
    while(i<=p){
        mes.insert(i*13,"\n");
        i++;
    }
    connect(&t,&QTimer::timeout,ui->hlepmes,[&](){
        n++;
        if(n!=m)
        {
            ui->hlepmes->setText(mes.left(n));
            ui->hlepmes->adjustSize();
            t.start(2000/m);
        }
    });
    t.start(2000/m);
    QEventLoop loop;
    QTimer::singleShot(2000,&loop,SLOT(quit()));
    loop.exec();
}




void MainWindow::receiveMessage(QByteArray arr)
{
        QString ss=QString(arr.data());
        int i=ss.section("##",0,0).toInt();
        QString s=QString(arr.data()).section("##",1,1);
        switch (i) {
        case 0://连接服务器失败
            ui->stackedWidget->setCurrentIndex(0);
            sendhelp(s);
            break;
        case 1://连接服务器成功
            client->socket->pername=ui->name->text();
            client->socket->Pix=*Pix;
            client->sendMessagetos(COMM_CLIENT_IP,ip+"##"+client->socket->pername);
            sendhelp(s);
            client->sendpixtos();
            break;
        case 2://服务器关闭
            delete client;
            client=0;
            ui->all->show();
            ui->stackedWidget->setCurrentIndex(0);
            ui->imputdk->clear();
            sendhelp("服务器关闭！！！");
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
            ui->all->hide();
            client->game=new Gamemodel(client);
            client->game->type=MM;
            client->game->player1=new GPlayer(client->myflag,client->game,client->game,client->socket->pername);
            client->game->c=new Chessboard(this,client->game);
            connect(this,SIGNAL(sendsetmes(QPixmap,QString,QPixmap,QString)),client->game->c,SLOT(setmes(QPixmap,QString,QPixmap,QString)),Qt::QueuedConnection);
            sendsetmes(client->socket->Pix,client->socket->pername,client->pix,s);
            connect(this,SIGNAL(receivemeschat(QString)),client->game->c,SLOT(receivemeschat(QString)),Qt::QueuedConnection);
            connect(client->game->c,SIGNAL(sendmesschat(QString)),client,SLOT(sendMesschat(QString)),Qt::QueuedConnection);
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
                ui->all->show();
                ui->stackedWidget->setCurrentIndex(1);

            });
            client->game->start();
            client->game->c->show();
            break;
        case 5://某方玩家胜利，服务器结束游戏
        {
            QString xy=QString(arr.data()).section("##",2,2);
            if(!xy.isEmpty())
                client->game->state=client->game->GameEnd(xy.section("//",0,0).toInt(),xy.section("//",1,1).toInt());
            QMessageBox::information(NULL,"游戏结束",s);
            client->game->stop();
            delete client->game;
            client->game=0;
        }
            ui->all->show();
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
            ui->all->show();
            ui->stackedWidget->setCurrentIndex(1);
            break;
        case 15://玩家游戏操作：发送聊天信息
            emit receivemeschat(s);
            break;
        case 16://玩家发来消息，对方退出游戏
            QMessageBox::information(NULL,"游戏结束",s);
            client->game->stop();
            delete client->game;
            client->game=0;
            ui->all->show();
            ui->stackedWidget->setCurrentIndex(1);
            break;
        case 18://玩家发送图片
            {
                QByteArray array=arr.mid(5);
                while(client->socket->waitForReadyRead(100)){
                    array.append((QByteArray)client->socket->readAll());
                }

                QBuffer buffer(&array);
                buffer.open(QIODevice::ReadOnly);
                QImageReader reader(&buffer,"JPG");
                QImage img = reader.read();
                if(!img.isNull()){
                    client->pix=QPixmap::fromImage(img);
                }
                else {
                    client->pix=QPixmap(":/reso/head-portrait/tx1.jpg");
                }
            }
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
    if(ui->imputdk->text().isEmpty()||ui->imputip->text().isEmpty()){
        sendhelp("端口和ip不能为空");
    }
    else
    {
        client=new Client(ui->imputdk->text(),ui->imputip->text(),this);
        if(client->iscon)
        {
            ui->stackedWidget->setCurrentIndex(1);
            ui->tx_2->setPixmap(*Pix);
            ui->name_2->setText(ui->name->text());
            connect(this,&MainWindow::destroyed,client->socket,&QTcpSocket::disconnectFromHost);
        }
        else{
            delete client;
            client=0;
            sendhelp("服务器连接失败（可能无访问权限或该端口服务器不存在）");
        }
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
        ui->btopen->setText("开房");
        sendhelp("断开与服务器连接");

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
    if(item->text().section(" ",0,0)==ip) return;
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

void MainWindow::on_toolButton_clicked()
{
    client->sendMessagetos(COMM_CLIENT_REFLASH,"");
    if(client->game==0) ui->btopen->setText("开房");
}



void MainWindow::showEvent(QShowEvent *)
{
    if(permes!=0) return;
    sendhelp("你好，我是你的助手-喻小雨");
    permes=new QFile(Client::apppath+"//perinf.txt",this);
    if(permes->open(QIODevice::ReadWrite)){
        QString mes=QString(permes->readAll().data());
        QString name=mes.section(" ",0,0);
        QString txpath=mes.section(" ",1,1);
        Pix=new QPixmap;
        if(!Pix->load(txpath)){
            sendhelp("头像加载失败,使用默认头像");
            Pix->load(":/reso/head-portrait/tx1.jpg");
        }
        ui->name->setText(name);
    }
    else{
        sendhelp("信息加载失败，找不到路径,已使用默认信息");
        Pix=new QPixmap (":/reso/head-portrait/tx1.jpg");
        ui->btupdatemes->setEnabled(!true);
    }
    ui->tx->setPixmap(*Pix);
    ui->tx->setScaledContents(true);
}


void MainWindow::on_btupdatemes_clicked()
{
    if(ui->btupdatemes->text()=="修改"){
        ui->name->setReadOnly(!true);
        ui->txpath->show();
        ui->btupdatemes->setText("保存");
        sendhelp("如需换头像，请在上方框中输入图片绝对路径，注意路径中不能有空格和中文");
        return;
    }
    if(ui->btupdatemes->text()=="保存"){
        if(ui->name->text().isEmpty()) {sendhelp("用户名不能为空！");return;}
        if(ui->name->text().contains(" ")){sendhelp("用户名不能包含空格");return;}
        if(ui->txpath->text().contains(" ")){sendhelp("路径不能包含空格");return;}
        if(!ui->txpath->text().contains("jpg")){sendhelp("请使用.jpg格式图片");return;}
        if(ui->txpath->text().contains(QRegExp("[\\x4e00-\\x9fa5]+"))){sendhelp("路径不能带中文");return;}

        if(ui->txpath->text().isEmpty()){
            QString file=QString(permes->readAll().data());
            QString path=file.section(" ",1,1);
            permes->close();
            if(permes->open(QIODevice::ReadWrite|QIODevice::Truncate))
            {
                permes->write(QString(ui->name->text()+" "+path).toLatin1());
            }
        }
        else{
            QString p=ui->txpath->text();
            if(!Pix->load(p)){
                sendhelp("图片加载失败");
                return;
            }
            permes->close();
            if(permes->open(QIODevice::ReadWrite|QIODevice::Truncate))
            {

                permes->write(QString(ui->name->text()+" "+p).toLatin1());
            }

        }
        ui->txpath->hide();
        ui->name->setReadOnly(true);
        ui->btupdatemes->setText("修改");
        ui->tx->setPixmap(*Pix);

    }
}

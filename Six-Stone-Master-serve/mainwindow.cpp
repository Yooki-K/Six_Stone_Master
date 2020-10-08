#include "mainwindow.h"
#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)//建立玩家选择界面，并且设置动态背景
{
    ui->setupUi(this);
    f=new Selectform(this);
    MOVETOCENTER(f);
    connect(f,SIGNAL(sendmes(Gametype,GameAI)),this,SLOT(receimes(Gametype,GameAI)));//将select界面与棋盘界面进行数据传递
    connect(f,SIGNAL(online()),this,SLOT(openline()));
    //设置动态背景
    QLabel* myback = new QLabel(this);
    myback->setGeometry(0,40,WIDTH,HEIGHT-80);
    QMovie * move = new QMovie(":/new/myresouce/reso/gif/lion.gif");
    myback->setMovie(move);
    myback->setScaledContents(true);//大小自适应
    move->start();
    connect(move,&QMovie::finished,move,[&](){move->start();});//动态背景循环
    myback->lower();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *)
{
    if(server==nullptr) return;
    for(int i=0;i<server->clientlist.size();i++){
        server->sendMestoc(server->clientlist.at(i),COMM_SERVER_CLOSE,"端口"+QString::number(server->serverdk)+"服务器关闭");
        server->clientlist.at(i)->waitForBytesWritten();
        if(server->clientlist.at(i)->match!=0){
            server->clientlist.at(i)->match->clear();
            server->clientlist.at(i)->clear(1);
        }
    }
    delete server;
    server=nullptr;
}

void MainWindow::receimes(Gametype t1, GameAI ai1)//接收玩家选择的模式和难度
{
    localgame=new Gamemodel(this);//建立线程游戏
    localgame->c=new Chessboard(this,localgame);//建立棋盘界面
    localgame->c->show();
    localgame->type=t1;   localgame->AItype=ai1;//设置游戏模式
}

// ////////////////////////////////////////////////////////////////

void MainWindow::openline()//开启服务器，并将必要信号和槽函数连接的槽函数
{
    sf=new SeverForm(this);//建立服务器页面
    server=new Server(sf,this);//建立服务器
    sf->server=server;
    connect(sf,&SeverForm::closesever,this,[&](){
        server->isaccord=1;
        for(int i=0;i<server->clientlist.size();i++){
            server->sendMestoc(server->clientlist.at(i),COMM_SERVER_CLOSE,"端口"+QString::number(server->serverdk)+"服务器关闭");
            server->clientlist.at(i)->waitForBytesWritten();
            if(server->clientlist.at(i)->match!=0){
                server->clientlist.at(i)->match->clear();
                server->clientlist.at(i)->clear(1);
            }
        }
        server->close();
        delete server;
        server=0;
        f->show();
        sf->close();
    },Qt::QueuedConnection);//传输关闭服务器命令给每个连接的客户端并断开连接,关闭服务器
    connect(this,&MainWindow::destroyed,sf,[&](){
        emit sf->closesever();
    });//关闭窗口，触发关闭服务器信号
    connect(server,SIGNAL(sendupdateGameInfo(Server*)),sf,SLOT(updateroom(Server*)),Qt::QueuedConnection);//修改游戏大厅
    connect(server,&Server::openroom,sf,&SeverForm::btopenclicked);//开房
    connect(server,SIGNAL(sendupdatenum(int)),sf,SLOT(updatenum(int)));//更改当前连接人数
    connect(server,&Server::sendupdatemesbox,sf,&SeverForm::updatemesbox);//传达客户端连接断开消息
    connect(server,&Server::sendupdatemesbox,sf,&SeverForm::sendhelp);//传达客户端连接断开信息给小助手
    sf->show();
}

void MainWindow::GameOver(int state, bool flag)//本地端游戏结束函数
{
    if(server!=0){//服务器开启则直接返回
        return;
    }
    if(localgame!=nullptr)
    {
        if(localgame->isover) return;
        localgame->isover=1;
        QString gameend;
        if((Gamestate)state==win) {
                if(!flag) gameend="白方";
                else     gameend="黑方";
            gameend+="获胜,是否重新开始游戏";
        }
        else gameend="双方和棋,是否重新开始游戏";
        localgame->stop();
        QMessageBox:: StandardButton result=QMessageBox::information(NULL, "游戏结束", gameend,QMessageBox::Yes|QMessageBox::No);
        if(result==QMessageBox::Yes)
        {
            delete localgame;
            localgame=nullptr;
            f->show();
        }//回到选择界面
        else{
            delete localgame;
            localgame=nullptr;
            exit(0);
        }//退出程序
    }
}





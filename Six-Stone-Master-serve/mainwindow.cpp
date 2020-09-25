#include "mainwindow.h"
#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    f=new Selectform(this);
    MOVETOCENTER(f);
    connect(f,SIGNAL(sendmes(Gametype,GameAI)),this,SLOT(receimes(Gametype,GameAI)));//将select界面与棋盘界面进行数据传递
    connect(f,SIGNAL(online()),this,SLOT(openline()));
    ui->label->hide();
    ui->label->setText("服务器本地端口");
    ui->label->move(WIDTH-ui->label->width()-150,20);
    ui->label->setAlignment(Qt::AlignHCenter);
    //设置动态背景
    QLabel* myback = new QLabel(this);
    myback->setGeometry(0,0,WIDTH,HEIGHT);
    QMovie * move = new QMovie(":/new/myresource/reso/gif/star1.gif");
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

void MainWindow::receimes(Gametype t1, GameAI ai1)
{
    localgame=new Gamemodel(this);//建立线程游戏
    localgame->c=new Chessboard(this,localgame);
    localgame->c->show();
    ui->label->show();
    localgame->type=t1;
    localgame->AItype=ai1;
}

// ////////////////////////////////////////////////////////////////

void MainWindow::openline()
{
    sf=new SeverForm(this);//建立服务器页面
    server=new Server(sf,this);//建立服务器
    sf->server=server;
    connect(sf,&SeverForm::closesever,this,[&](){
        server->isaccord=1;
        for(int i=0;i<server->clientlist.size();i++){
            this->server->sendMestoc(server->clientlist.at(i),COMM_SERVER_CLOSE,"端口"+QString::number(server->serverdk)+"服务器关闭");
            server->clientlist.at(i)->close();
        }
        server->close();
        delete server;
        server=0;
        f->show();
        sf->close();
    });//传输关闭服务器命令给每个连接的客户端并断开连接,关闭服务器
    connect(this,&MainWindow::destroyed,sf,[&](){
        emit sf->closesever();
    });
    connect(server,SIGNAL(senddktoui(QString)),this,SLOT(updatelabel(QString)));
    connect(server,SIGNAL(sendupdateGameInfo(Server*)),sf,SLOT(updateroom(Server*)));//修改游戏大厅
    connect(server,&Server::openroom,sf,&SeverForm::btopenclicked);
    connect(server,SIGNAL(sendupdatenum(int)),sf,SLOT(updatenum(int)));//更改当前连接人数
    sf->show();
    MOVETOCENTER(sf);
    ui->label->show();
}

void MainWindow::updatelabel(QString dk)
{
    ui->label->setText("当前服务器端口:"+dk);
}
void MainWindow::GameOver(int state, bool flag)
{
    if(server->mysocket!=0){
        return;
    }
    if(localgame!=nullptr)
    {
        QString gameend;
        if((Gamestate)state==win) {
            if(!flag) gameend="白方";
            else     gameend="黑方";
            gameend+="获胜,是否重新开始游戏";
        }
        else gameend="双方和棋,是否重新开始游戏";
        QMessageBox:: StandardButton result=QMessageBox::information(NULL, "游戏结束", gameend,QMessageBox::Yes|QMessageBox::No);
        if(result==QMessageBox::Yes)//只是重新开始游戏，游戏模式不变
        {
            localgame->stop();
            delete localgame;
            localgame=nullptr;
            f->show();
        }
        else{
            exit(0);
        }
    }
}





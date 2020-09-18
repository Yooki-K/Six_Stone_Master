#include "mainwindow.h"
#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    f=new Selectform(this);
    f->move(WIDTH/2-f->width()/2,HEIGHT/2-f->height()/2);
    connect(f,SIGNAL(sendmes(Gametype,GameAI)),this,SLOT(receimes(Gametype,GameAI)));//将select界面与棋盘界面进行数据传递
    connect(f,SIGNAL(online()),this,SLOT(openline()));
    ui->label->hide();
    ui->toserve->hide();
    ui->label->setText("服务器本地端口");
    ui->label->move(WIDTH-ui->label->width()-150,20);
    ui->label->setAlignment(Qt::AlignHCenter);
    ui->toserve->move(WIDTH-100,HEIGHT-50);
    //设置动态背景
    QLabel* myback = new QLabel(this);
    myback->setGeometry(0,0,WIDTH,HEIGHT);
    QMovie * move = new QMovie(":/new/background/reso/star1.gif");
    myback->setMovie(move);
    myback->setScaledContents(true);//大小自适应
    move->start();
    connect(move,&QMovie::finished,move,[=](){move->start();});//动态背景循环
    myback->lower();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::receimes(Gametype t1, GameAI ai1)
{
    localgame=new Gamemodel(this);//建立线程游戏
    ui->label->show();
    localgame->type=t1;
    localgame->AItype=ai1;
}

// ////////////////////////////////////////////////////////////////

void MainWindow::openline()
{
    sf=new SeverForm(this);//建立服务器页面
    server=new Server(sf);//建立服务器
    connect(sf,&SeverForm::closesever,this,[&](){
        for(int i=0;i<server->clientlist.size();i++){
            this->server->sendMestoc(server->clientlist.at(i),COMM_CLIENT_LOSE,"端口"+QString::number(server->serverdk)+"服务器关闭");
            server->clientlist.at(i)->close();
        }
        server->close();
    });//传输关闭服务器命令给每个连接的客户端并断开连接,关闭服务器
    connect(server,SIGNAL(sendupdatenum(int)),sf,SLOT(updatenum(int)));//更改当前连接人数
    connect(sf,SIGNAL(openroom(QString)),this,SLOT(openroom(QString)));//服务器本地开房
    connect(server,SIGNAL(openroom(QString)),this,SLOT(openroom(QString)));//客户端开房
    connect(sf,SIGNAL(connectroom(QString)),this,SLOT(connectroom(QString)));//加入本地房间
    connect(server,&Server::isokon,this,[&](){
        emit isokon();
    });//发送可以下棋信号
    sf->show();
    sf->move(WIDTH/2-sf->width()/2,HEIGHT/2-sf->height()/2);
    ui->label->show();
//    ui->toserve->show();
}

void MainWindow::updatelabel(QString dk)
{
    ui->label->setText("当前服务器端口:"+dk);
}

void MainWindow::GameOver(Gamestate state, bool flag)
{

    QString gameend;
    if(state==win) {
        if(!flag) gameend="白方";
        else     gameend="黑方";
        gameend+="获胜,是否重新开始游戏";
    }
    else gameend="双方和棋,是否重新开始游戏";
    QMessageBox:: StandardButton result=QMessageBox::information(NULL, "游戏结束", gameend,QMessageBox::Yes|QMessageBox::No);
    if(result==QMessageBox::Yes)//只是重新开始游戏，游戏模式不变
    {
        f->show();
     }
   else{
       exit(0);
    }

}

void MainWindow::connectroom(QString tocon)
{
    int i=server->playerFightInfo.indexOf(QPair<QString,QString>(tocon,"-"));
    if(i!=-1){
        server->playerFightInfo[i].second="127.0.0.1";
        emit server->sendupdateGameInfo(server);
        QList<MySocket*>allTcpSocket=server->findChildren<MySocket*>();
        for(int j=0;j<allTcpSocket.size();j++)
           {
               if(allTcpSocket.at(j)->peerAddress().toString().mid(7)==server->playerFightInfo.at(i).first)
               {
                   server->servergame=new Gamemodel((QWidget *)this->parent());//在主窗口中建立线程游戏
                   server->servergame->type=MM;
                   server->servergame->AItype=none;
                   server->servergame->isonline=1;
                   //bool,Gamemodel *game=0, QObject *parent=0 ,  QString name="Player"
                   server->servergame->player1=new GPlayer(1,server->servergame,server->servergame,tocon);
                   server->servergame->player2=new GPlayer(0,server->servergame,server->servergame,"127.0.0.1");
                   allTcpSocket.at(j)->game=server->servergame;
                   allTcpSocket.at(j)->match=allTcpSocket.at(j);
                   server->mysocket=allTcpSocket.at(j);
                   server->serverplayer=server->servergame->player1;
                   allTcpSocket.at(j)->my=server->servergame->player2;
               }
           }
        server->servergame->c=new Chessboard(this);
        server->servergame->c->show();
        server->servergame->start();
        emit isokon();
   }
}

void MainWindow::openroom(QString ip)
{
    if(isserveropenroom)
    {
        QMessageBox::information(this,"警告","服务器只能开启一个房间");
        return;
    }
    isserveropenroom=1;
    if(ip=="127.0.0.1") {
        ui->label->show();
    }
    server->playerFightInfo.push_back(qMakePair<QString,QString>(ip,"-"));
    emit server->sendupdateGameInfo(server);
}

void MainWindow::receiprogress(QString p)
{
    server->sendMestoc(server->mysocket,COMM_CLIENT_GAMEOP,p);
}


void MainWindow::on_pushButton_clicked()//显示游戏大厅
{
    sf->show();
}

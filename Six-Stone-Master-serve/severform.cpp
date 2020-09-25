#include "severform.h"
#include "ui_severform.h"

SeverForm::SeverForm(QWidget *parent) ://parent为mainwindow
    QWidget(parent),
    ui(new Ui::SeverForm)
{
    ui->setupUi(this);
    setWindowTitle("六子棋游戏大厅");
    connect(ui->btopenroom,&QPushButton::clicked,this,[&](){
        btopenclicked();
    });
}

SeverForm::~SeverForm()
{
    delete ui;
}

void SeverForm::on_btcloses_clicked()
{
    emit closesever();
    ui->currconnect->setText("当前连接人数(最大连接数为4)："+QString::number(0));
    ui->playerroom->clear();
}

void SeverForm::updateroom(Server *server)
{
    int i=server->playerFightInfo.size();
    QStringList player;
    for(int j=0;j<i;j++){
        player<<server->playerFightInfo[j].first+" PK "+server->playerFightInfo[j].second;
    }
    ui->playerroom->clear();
    ui->playerroom->addItems(player);
}

void SeverForm::on_playerroom_itemDoubleClicked(QListWidgetItem *item)//双击加入房间
{
    QString tocon(item->text().section(' ',0,0));
    int i=server->playerFightInfo.indexOf(QPair<QString,QString>(tocon,"-"));
    if(i!=-1){
        server->mysocket=new MySocket(this);
        server->mysocket->ip=IP;
        server->playerFightInfo[i].second=server->mysocket->ip;
       emit server->sendupdateGameInfo(server);

        QList<MySocket*>allTcpSocket=server->findChildren<MySocket*>();
        for(int j=0;j<allTcpSocket.size();j++)
           {
               if(allTcpSocket.at(j)->ip==server->playerFightInfo.at(i).first)
               {
                   server->mysocket->game=new Gamemodel(server,0);//在主窗口中建立线程游戏
                   server->mysocket->game->type=MM;
                   server->mysocket->game->AItype=none;
                   connect(server->mysocket->game,&Gamemodel::sendback,this,[&](){
                       server->sendMestoc(server->mysocket->match,COMM_CLIENT_UNDO,"对方请求悔棋，是否同意");
                   });
                   connect(server->mysocket->game,&Gamemodel::sendgv,this,[&](){
                       server->clearroom(server->mysocket);
                       server->sendMestoc(server->mysocket->match,COMM_CLIENT_LOSE,"对方认输，你赢了");
                       server->mysocket->match->clear();
                       server->mysocket->clear(1);
                       delete server->mysocket;
                       server->mysocket=0;
                   });
                   //bool,Gamemodel *game=0, QObject *parent=0 ,  QString name="Player"
                   server->mysocket->game->player1=new GPlayer(0,server->mysocket->game,server->mysocket->game,server->mysocket->ip);
                   server->mysocket->game->player2=new GPlayer(1,server->mysocket->game,server->mysocket->game,tocon);
                   allTcpSocket.at(j)->game=server->mysocket->game;
                   allTcpSocket.at(j)->match=allTcpSocket.at(j);
                   server->mysocket->match=allTcpSocket.at(j);
                   allTcpSocket.at(j)->match=server->mysocket;
                   allTcpSocket.at(j)->my=server->mysocket->game->player2;
                   connect(server->mysocket->game->c,SIGNAL(sendmesschat(QString)),server,SLOT(sendmesschat(QString)));
                   connect(server,SIGNAL(updatechat(QString)),server->mysocket->game->c,SLOT(updatechat(QString)));
                    server->mysocket->game->start();
                   QEventLoop loop;
                   QTimer::singleShot(1000,&loop,SLOT(quit()));
                   loop.exec();//等待一秒后发送游戏开始消息（下策）
                   server->sendMestoc(allTcpSocket.at(j),COMM_SERVER_GAMESTART,"");

               }
           }
   }
}


void SeverForm::btopenclicked(QString ip,QString text)//开房
{
    if(ip==IP)
    {
        if(ui->btopenroom->text()==QString("开房"))
        {
            server->mysocket=new MySocket(this);
            server->mysocket->ip=ip;
            server->playerFightInfo.push_back(qMakePair<QString,QString>(ip,"-"));
            emit server->sendupdateGameInfo(server);
            ui->btopenroom->setText("关房");
        }
        else{
            ui->btopenroom->setText("开房");
            server->clearroom(ip);
        }
    }
    else{
        if(text==QString("开房"))
        {
            server->playerFightInfo.push_back(qMakePair<QString,QString>(ip,"-"));
            emit server->sendupdateGameInfo(server);
        }
        else
            server->clearroom(ip);
    }   
}

void SeverForm::updatenum(int num)
{
    ui->currconnect->setText("当前连接人数(最大连接数为2)："+QString::number(num));
}






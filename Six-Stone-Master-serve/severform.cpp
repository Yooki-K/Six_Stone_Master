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
    },Qt::QueuedConnection);
    ui->txpath->hide();
    ui->mesbox->setReadOnly(true);
    ui->tx->setScaledContents(true);
    ui->name->setReadOnly(true);
    yuxiaoyu=new QMovie(":/new/myresouce/reso/head-portrait/yuyan.gif",QByteArray(),this);//小助手动图设置
    ui->helper->setMovie(yuxiaoyu);
    ui->helper->setScaledContents(true);
}

SeverForm::~SeverForm()
{
     if(Pix!=0) {delete Pix;Pix=0;}
    if(permes->isOpen()) permes->close();//如果文本文件未关闭，则关闭文本文件
    delete ui;
}

void SeverForm::on_btcloses_clicked()
{
    emit closesever();
    ui->currconnect->setText("当前连接人数(最大连接数为2)：0");
    ui->playerroom->clear();
    ui->btopenroom->setText("开房");
}

void SeverForm::updateroom(Server *server)//更改房间信息函数
{
    int i=server->playerFightInfo.size();
    QStringList player;
    for(int j=0;j<i;j++){
        player<<server->playerFightInfo[j].first+" PK "+server->playerFightInfo[j].second;
    }
    ui->playerroom->clear();//清空
    ui->playerroom->addItems(player);//重新添加
}

void SeverForm::updatenum(int num)//更改连接人数
{
    ui->currconnect->setText("当前连接人数(最大连接数为2)："+QString::number(num));
}

void SeverForm::updatemesbox(QString mes)//消息框添加客户端连接断开信息
{
    ui->mesbox->append(QTime::currentTime().toString()+" "+mes);
}
void SeverForm::on_playerroom_itemDoubleClicked(QListWidgetItem *item)//玩家双击加入房间函数，与Server类receiveMesfromc函数中加入房间类似
{
    QString tocon(item->text().section(' ',0,0));
    if(tocon==IP) return;
    int i=server->playerFightInfo.indexOf(QPair<QString,QString>(tocon,"-"));
    if(i!=-1){
        server->playerFightInfo[i].second=server->mysocket->ip;
       emit server->sendupdateGameInfo(server);

        for(int j=0;j<server->clientlist.size();j++)
           {
               if(server->clientlist.at(j)->ip==server->playerFightInfo.at(i).first)
               {
                   server->mysocket->game=new Gamemodel(server,0);//在主窗口中建立线程游戏
                   server->mysocket->game->type=MM;
                   server->mysocket->game->AItype=none;
                   //bool,Gamemodel *game=0, QObject *parent=0 ,  QString name="Player"
                   server->mysocket->game->player1=new GPlayer(0,server->mysocket->game,server->mysocket->game,server->mysocket->pername);
                   server->mysocket->game->player2=new GPlayer(1,server->mysocket->game,server->mysocket->game,server->clientlist.at(j)->pername);
                   server->mysocket->game->c->setbegin();
                   server->clientlist.at(j)->game=server->mysocket->game;
                   server->clientlist.at(j)->match=server->clientlist.at(j);
                   server->mysocket->match=server->clientlist.at(j);
                   server->clientlist.at(j)->match=server->mysocket;
                   server->clientlist.at(j)->my=server->mysocket->game->player2;
                   connect(server->mysocket->game,&Gamemodel::sendback,this,[&](){
                       server->sendMestoc(server->mysocket->match,COMM_CLIENT_UNDO,"对方请求悔棋，是否同意");
                   },Qt::QueuedConnection);
                   connect(server->mysocket->game,&Gamemodel::sendgv,this,[&](){
                       server->clearroom(server->mysocket);
                       server->sendMestoc(server->mysocket->match,COMM_CLIENT_LOSE,"对方认输，你赢了");
                       server->mysocket->match->clear();
                       server->mysocket->clear(1);
                   },Qt::QueuedConnection);
                   emit sendsetmes(server->mysocket->Pix,server->mysocket->pername,server->mysocket->match->Pix,server->mysocket->match->pername);
                   connect(server->mysocket->game->c,SIGNAL(sendmesschat(QString)),server,SLOT(sendmesschat(QString)),Qt::QueuedConnection);
                   connect(server,SIGNAL(updatechat(QString)),server->mysocket->game->c,SLOT(updatechat(QString)),Qt::QueuedConnection);
                    server->mysocket->game->start();
                   server->sendpixtoc();
                   server->waits(500);
                   server->sendMestoc(server->clientlist.at(j),COMM_SERVER_GAMESTART,server->mysocket->pername);

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
            ui->btopenroom->setText("关房");
            server->playerFightInfo.push_back(qMakePair<QString,QString>(ip,"-"));
            emit server->sendupdateGameInfo(server);
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



void SeverForm::sendhelp(QString mes)//小助手提示对话函数
{
    yuxiaoyu->start();//播放动图
    QTimer t;
    int n=0;
    int m=mes.toLocal8Bit().size();
    int p=m/26;
    int i=1;
    if(m<=0) return;
    while(i<=p){
        mes.insert(i*13,"\n");//13个字加一个换行符
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
    });//利用计时器，启到对话随时间显示效果
    t.start(2000/m);
    QEventLoop loop;
    QTimer::singleShot(2000-2000/mes.size(),&loop,SLOT(quit()));//等待小助手提示完毕
    loop.exec();
}

void SeverForm::on_btreflash_clicked()
{
    emit server->sendupdateGameInfo(server);
    ui->btopenroom->setText("开房");
    int i=server->playerFightInfo.indexOf(QPair<QString,QString>(server->mysocket->ip,"-"));
    if(i!=-1) ui->btopenroom->setText("关房");
}

void SeverForm::on_btupdatemes_clicked()//修改个人信息函数
{
    if(ui->btupdatemes->text()=="修改"){
        ui->name->setReadOnly(!true);
        ui->txpath->show();
        ui->btupdatemes->setText("保存");
        sendhelp("如需换头像，请在上方框中输入图片绝对路径，注意路径中不能有空格和中文,图片请使用jpg格式");
        return;
    }
    if(ui->btupdatemes->text()=="保存"){
        //对用户名和图片路径进行格式设置
        if(ui->name->text().isEmpty()) {sendhelp("用户名不能为空！");return;}
        if(ui->name->text().contains(" ")){sendhelp("用户名不能包含空格");return;}
        if(ui->txpath->text().contains(" ")){sendhelp("路径不能包含空格");return;}
        if(!ui->txpath->text().contains("jpg")){sendhelp("请使用.jpg格式图片");return;}
        if(ui->txpath->text().contains(QRegExp("[\\x4e00-\\x9fa5]+"))){sendhelp("路径不能带中文");return;}
        if(ui->txpath->text().isEmpty()){
            QString file=QString(permes->readAll().data());
            QString path=file.section(" ",1,1);
            if(permes->open(QIODevice::ReadWrite|QIODevice::Truncate)){
                permes->write(QString(ui->name->text()+" "+path).toLatin1());
                permes->close();
            }
        }
        else{
            QString p=ui->txpath->text();
            if(!Pix->load(p)){
                sendhelp("图片加载失败,建议手打输入");
                return;
            }
            if(permes->open(QIODevice::ReadWrite|QIODevice::Truncate)){
                permes->write(QString(ui->name->text()+" "+p).toLatin1());
                permes->close();
            }
        }
        ui->txpath->hide();
        ui->name->setReadOnly(true);
        ui->btupdatemes->setText("修改");
        ui->tx->setPixmap(*Pix);
        server->mysocket->Pix=*Pix;
        server->mysocket->pername=ui->name->text();
    }
}

void SeverForm::showEvent(QShowEvent *)//界面显示时，加载玩家个人信息
{
  MOVETOCENTER(this);//界面放屏幕中心（定义的宏函数）
  if(permes!=0) return;//如果不是初次显示，则直接返回
  sendhelp("你好，我是你的助手-喻小雨");
  permes=new QFile(Server::apppath+"//perinf.txt",this);
  if(permes->open(QIODevice::ReadWrite)){
      QString mes=QString(permes->readAll().data());
      QString name=mes.section(" ",0,0);
      QString txpath=mes.section(" ",1,1);
      Pix=new QPixmap;
      if(!Pix->load(txpath)){
          sendhelp("头像加载失败,使用默认头像");
          Pix->load(":/new/myresouce/reso/head-portrait/tx1.jpg");
      }
      ui->name->setText(name);
      permes->close();
  }
  else{//文件打开失败
      sendhelp("信息加载失败，找不到路径,\n已使用默认信息");
      Pix=new QPixmap (":/new/myresouce/reso/head-portrait/tx1.jpg");
      ui->btupdatemes->setEnabled(!true);
  }
  ui->tx->setPixmap(*Pix);
  ui->tx->setScaledContents(true);
  server->mysocket->Pix=*Pix;
  server->mysocket->pername=ui->name->text();
}

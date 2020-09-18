#include "severform.h"
#include "ui_severform.h"

SeverForm::SeverForm(QWidget *parent) ://parent为mainwindow
    QWidget(parent),
    ui(new Ui::SeverForm)
{
    ui->setupUi(this);
    setWindowTitle("六子棋游戏大厅");
}

SeverForm::~SeverForm()
{
    delete ui;
}

void SeverForm::on_btcloses_clicked()
{
    emit closesever();
    ui->playerroom->clear();
}

void SeverForm::updateroom(Server *server)
{
    ui->playerroom->clear();
    int i=server->playerFightInfo.size();
    QStringList player;
    for(int j=0;j<i;j++){
        player<<server->playerFightInfo[j].first+" PK "+server->playerFightInfo[j].second;
    }
    ui->playerroom->addItems(player);
}

void SeverForm::on_playerroom_itemDoubleClicked(QListWidgetItem *item)//双击加入房间
{
    QString toconnect(item->text().section(' ',0,0));
    emit connectroom(toconnect);
}


void SeverForm::on_btopenroom_clicked()//开房
{
    emit openroom("127.0.0.1");
}

void SeverForm::updatenum(int num)
{
    ui->currconnect->setText("当前连接人数："+QString::number(num));
}






#ifndef SEVERFORM_H
#define SEVERFORM_H
#include<QListWidgetItem>
#include <QWidget>
#include"server.h"
#include<QFile>
#include<QMovie>
#include<QRegExp>
namespace Ui {
class SeverForm;
}

class SeverForm : public QWidget
{
    Q_OBJECT
private:
    Ui::SeverForm *ui;
    QMovie *yuxiaoyu=0;
    QFile *permes=0;
    QPixmap *Pix=0;
public:
    explicit SeverForm(QWidget *parent = 0);
    ~SeverForm();
    Server*server=0;

private slots:
    void on_btcloses_clicked();
    void updateroom(Server *);

    void on_playerroom_itemDoubleClicked(QListWidgetItem *item);


    void on_btreflash_clicked();

    void on_btupdatemes_clicked();
    void showEvent(QShowEvent*);

public slots:
    void btopenclicked(QString ip="127.0.0.1", QString text="开房");
    void updatenum(int);
    void updatemesbox(QString);
    void sendhelp(QString);

signals:
    void closesever();
    void openroom(QString);
    void closeroom();
    void connectroom(QString);
    void sendsetmes(QPixmap,QString,QPixmap,QString);
};

#endif // SEVERFORM_H

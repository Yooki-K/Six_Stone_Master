#ifndef SEVERFORM_H
#define SEVERFORM_H
#include<QListWidgetItem>
#include <QWidget>
#include"server.h"
namespace Ui {
class SeverForm;
}

class SeverForm : public QWidget
{
    Q_OBJECT
private:
    Ui::SeverForm *ui;
public:
    explicit SeverForm(QWidget *parent = 0);
    ~SeverForm();
    Server*server=0;

private slots:
    void on_btcloses_clicked();
    void updateroom(Server *);

    void on_playerroom_itemDoubleClicked(QListWidgetItem *item);

public slots:
    void btopenclicked(QString ip="127.0.0.1", QString text="开房");
    void updatenum(int);
signals:
    void closesever();
    void openroom(QString);
    void closeroom();
    void connectroom(QString);
};

#endif // SEVERFORM_H

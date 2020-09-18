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

public:
    explicit SeverForm(QWidget *parent = 0);
    ~SeverForm();
    Ui::SeverForm *ui;
private slots:
    void on_btcloses_clicked();
    void updateroom(Server *);

    void on_playerroom_itemDoubleClicked(QListWidgetItem *item);

    void on_pushButton_clicked();

    void on_btopenroom_clicked();

    void on_playerroom_doubleClicked(const QModelIndex &index);
    void updatenum(int);
signals:
    void closesever();
    void openroom(QString);
    void connectroom(QString);
};

#endif // SEVERFORM_H

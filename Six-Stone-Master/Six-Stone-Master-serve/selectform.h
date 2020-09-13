#ifndef SELECTFORM_H
#define SELECTFORM_H
#include <QWidget>
#include"gamemodel.h"
namespace Ui {
class Selectform;
}

class Selectform : public QWidget
{
    Q_OBJECT

public:
    explicit Selectform(QWidget *parent = 0);
    ~Selectform();
    void ButtonMMclicked();

    void ButtonMAclicked();

    void ButtonAAclicked();
    bool eventFilter(QObject *, QEvent *);

    void ButtonEasyclicked();

    void ButtonDiffclicked();

    void ButtonOnclicked();

private:
    Ui::Selectform *ui;
signals:
    void sendmes(Gametype,GameAI);
    void online();
};

#endif // SELECTFORM_H

#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include "QListWidgetItem"
#include "user.h"

namespace Ui {
class Login;
}

class Login : public QMainWindow
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();
     QVector <User*> itemVec;

private slots:
    void on_pushButton_clicked();
    void on_caltest_clicked();

    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_calltest_clicked();

    void on_mute_clicked();

    void on_deco_clicked();

public slots:
    Q_SLOT void hideThisWidget();

private:
    Ui::Login *ui;
};

#endif // LOGIN_H

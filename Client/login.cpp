#include "login.h"
#include "iostream"
#include "ui_login.h"
#include "QGraphicsOpacityEffect"
#include "QPropertyAnimation"


Login::Login(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);

}

Login::~Login()
{
    delete ui;
}

void Login::on_pushButton_clicked()
{
    if (ui->stackedWidget->currentIndex() == 0)
    {
    QGraphicsOpacityEffect *eff = new QGraphicsOpacityEffect(this);
    ui->label_5->setGraphicsEffect(eff);
    ui->frame_3->setGraphicsEffect(eff);
    QPropertyAnimation *a = new QPropertyAnimation(eff,"opacity");
    a->setDuration(500);
    a->setStartValue(1);
    a->setEndValue(0);
    a->setEasingCurve(QEasingCurve::OutBack);
    a->start(QPropertyAnimation::DeleteWhenStopped);
    connect(a,SIGNAL(finished()),this,SLOT(hideThisWidget()));
    }

}

void Login::hideThisWidget()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void Login::on_caltest_clicked()
{
    if(ui->label_8->geometry().y() < 0)
    {
    QPropertyAnimation *a = new QPropertyAnimation(ui->label_8,"geometry");
    a->setDuration(200);
    a->setStartValue(ui->label_8->geometry());
    a->setEndValue(QRect(910, 0 , 371 , 131));
    a->start();
    std::cout << "dqsdqsd" << std::endl;
    }
    else
    {
        std::cout << "qqsdqsd" << std::endl;
        QPropertyAnimation *a = new QPropertyAnimation(ui->label_8,"geometry");
        a->setDuration(200);
        a->setStartValue(ui->label_8->geometry());
        a->setEndValue(QRect(910, -131 , 371 , 131));
        a->start();
    }
}

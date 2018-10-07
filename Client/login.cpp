#include "login.h"
#include "iostream"
#include "ui_login.h"
#include "QGraphicsOpacityEffect"
#include "QPropertyAnimation"
#include "GlobalSession.h"
#include "Command.h"
#include "user.h"

Login::Login(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Login)
{

    ui->setupUi(this);
    client->ui = this->ui;
    client->w = this;
    ui->widget_2->setHidden(true);


}

Login::~Login()
{
    delete ui;
}

void Login::on_pushButton_clicked()
{
    std::cout << "clicked " << std::endl;
    ui->lineEdit->setDisabled(1);
    ui->lineEdit_2->setDisabled(1);
    std::string login;
    login = std::string(" ") + ui->lineEdit->text().toStdString() + std::string(" ") + ui->lineEdit_2->text().toStdString();
    Command command = {0};
    command.magic = COMMAND_MAGIC;
    command.data.id = CommandIdentifier::LOGIN;
    std::memcpy((char *)command.data.data + sizeof(CommandIdentifier), login.c_str(), login.size());
    client->deliver(BabelString((char *)&command, sizeof(Command)), true);


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
    }
    else
    {
        QPropertyAnimation *a = new QPropertyAnimation(ui->label_8,"geometry");
        a->setDuration(200);
        a->setStartValue(ui->label_8->geometry());
        a->setEndValue(QRect(910, -131 , 371 , 131));
        a->start();
    }
}

void Login::on_listWidget_itemClicked(QListWidgetItem *item)
{
    User *user = static_cast<User *>(ui->listWidget->itemWidget(item));
    ui->widget->setName(user->getName());
    if(ui->widget->geometry().x() == 1280)
    {
    QPropertyAnimation *a = new QPropertyAnimation(ui->widget,"geometry");
    a->setDuration(200);
    a->setStartValue(ui->widget->geometry());
    a->setEndValue(QRect(1039, 0 , 241 , 300));
    a->start();
    }

}

void Login::on_calltest_clicked()
{

    client->Call("82.255.137.138", 4242);

}

void Login::on_mute_clicked()
{
    client->muted = !client->muted;
}

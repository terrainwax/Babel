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
    ui->lineEdit->setDisabled(1);
    ui->lineEdit_2->setDisabled(1);
    std::string login;
    login = std::string(" ") + ui->lineEdit->text().toStdString() + std::string(" ") + ui->lineEdit_2->text().toStdString();
    Command command = {0};
    command.magic = COMMAND_MAGIC;
    command.data.id = CommandIdentifier::LOGIN;
    client->id = CommandIdentifier::LOGIN;
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

void Login::on_deco_clicked()
{
    ui->lineEdit->setDisabled(0);
    ui->lineEdit_2->setDisabled(0);
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    Command command = {0};
    command.magic = COMMAND_MAGIC;
    command.data.id = CommandIdentifier::OFFLINE;
    client->deliver(BabelString((char *)&command, sizeof(Command)), true);
    ui->stackedWidget->setCurrentIndex(0);
    QGraphicsOpacityEffect *eff = new QGraphicsOpacityEffect(this);
    ui->label_5->setGraphicsEffect(eff);
    ui->frame_3->setGraphicsEffect(eff);
    QPropertyAnimation *a = new QPropertyAnimation(eff,"opacity");
    a->setDuration(500);
    a->setStartValue(0);
    a->setEndValue(1);
    a->setEasingCurve(QEasingCurve::OutBack);
    a->start(QPropertyAnimation::DeleteWhenStopped);
    if (client->TcpClient->state() == QAbstractSocket::ConnectedState)
        client->TcpClient->disconnectFromHost();
    client->_secured = false;
    ClientCrypto crypto;
    client->_crypto = crypto;
    if (client->TcpClient->waitForDisconnected(60000)) {
        if (!client->Connect())
            exit(0);
    }
}

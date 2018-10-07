#include "incall.h"
#include "ui_incall.h"
#include "Command.h"
#include "BabelString.h"
#include "GlobalSession.h"

incall::incall(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::incall)
{
    ui->setupUi(this);
}

incall::~incall()
{
    delete ui;
}

void incall::setName(std::string name)
{
    ui->pseudo->setText(name.c_str());
}

void incall::on_endcall_clicked()
{
    client->incall = false;
    client->UdpClient->close();
    client->clientCall.clear();
    client->ui->widget_3->setHidden(true);
    Command c2 = {0};
    c2.magic = COMMAND_MAGIC;
    c2.data.id = CommandIdentifier::HANG;
    client->deliver(BabelString((char *)&c2, sizeof(Command)));
    Command c3 = {0};
    c3.magic = COMMAND_MAGIC;
    c3.data.id = CommandIdentifier::ONLINE;
    client->deliver(BabelString((char *)&c3, sizeof(Command)));

}

void incall::on_mute_clicked()
{
    client->muted = !client->muted;
}

void incall::on_soundmute_clicked()
{
    client->soundmuted = !client->soundmuted;
}

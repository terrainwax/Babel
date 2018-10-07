#include "receive.h"
#include "ui_receive.h"
#include "GlobalSession.h"
#include "Command.h"

receive::receive(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::receive)
{
    ui->setupUi(this);
}

receive::~receive()
{
    delete ui;
}

void receive::setName(std::string name)
{
    ui->pseudo->setText(name.c_str());
}

void receive::on_join_clicked()
{

    Command c2 = {0};
    c2.magic = COMMAND_MAGIC;
    c2.data.id = CommandIdentifier::JOIN;
    client->id = CommandIdentifier::JOIN;
    std::memcpy((char *)c2.data.data + sizeof(CommandIdentifier), ui->pseudo->text().toStdString().c_str(), ui->pseudo->text().toStdString().size());
    client->deliver(BabelString((char *)&c2, sizeof(Command)));
}

void receive::on_hang_clicked()
{
    Command c2 = {0};
    c2.magic = COMMAND_MAGIC;
    c2.data.id = CommandIdentifier::HANG;
    std::memcpy((char *)c2.data.data + sizeof(CommandIdentifier), ui->pseudo->text().toStdString().c_str(), ui->pseudo->text().toStdString().size());
    client->deliver(BabelString((char *)&c2, sizeof(Command)));
}

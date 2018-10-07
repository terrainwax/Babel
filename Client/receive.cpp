#include <ctime>
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

    std::srand(std::time(nullptr));
    int random_variable = std::rand();
    client->Myport = ((random_variable + 49152))%65535;
    std::string login = ui->pseudo->text().toStdString() + std::string(" ") + std::to_string(client->Myport);
    client->userwaited = ui->pseudo->text().toStdString();
    Command c2 = {0};
    c2.magic = COMMAND_MAGIC;
    c2.data.id = CommandIdentifier::JOIN;
    client->id = CommandIdentifier::JOIN;
    std::memcpy((char *)c2.data.data + sizeof(CommandIdentifier), login.c_str(), login.size());
    client->deliver(BabelString((char *)&c2, sizeof(Command)));
    client->ui->widget_2->setHidden(true);
    client->ui->widget_3->setName(ui->pseudo->text().toStdString());
    client->ui->widget_3->setHidden(false);
}

void receive::on_hang_clicked()
{
    Command c2 = {0};
    c2.magic = COMMAND_MAGIC;
    c2.data.id = CommandIdentifier::HANG;
    std::memcpy((char *)c2.data.data + sizeof(CommandIdentifier), ui->pseudo->text().toStdString().c_str(), ui->pseudo->text().toStdString().size());
    client->deliver(BabelString((char *)&c2, sizeof(Command)));
    client->ui->widget_2->setHidden(true);
}

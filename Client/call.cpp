#include <ctime>
#include "call.h"
#include "ui_call.h"
#include "Command.h"
#include "BabelString.h"
#include "GlobalSession.h"

call::call(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::call)
{
    ui->setupUi(this);
}

call::~call()
{
    delete ui;
}

void call::setName(std::string name)
{
    ui->pseudo->setText(name.c_str());
}

void call::on_call_2_clicked()
{
    if (!client->incall) {
        std::string login;
        std::srand(std::time(nullptr));
        int random_variable = std::rand();
        client->Myport = ((random_variable + 49152)) % 65535;
        login = ui->pseudo->text().toStdString() + std::string(" ") + std::to_string(client->Myport);
        client->userwaited = ui->pseudo->text().toStdString();
        Command command = {0};
        command.magic = COMMAND_MAGIC;
        command.data.id = CommandIdentifier::CALL;
        client->id = CommandIdentifier::CALL;
        std::memcpy((char *) command.data.data + sizeof(CommandIdentifier), login.c_str(), login.size());
        client->deliver(BabelString((char *) &command, sizeof(Command)), true);
    }
}

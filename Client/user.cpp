#include "user.h"
#include "ui_user.h"

User::User(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::User)
{
    ui->setupUi(this);
}

User::~User()
{
    delete ui;
}

void User::setStatus(std::string status) {
if (status == std::string("Available"))
{
    ui->label->setStyleSheet("QLabel { background-color : green; border-radius:20px;}");
}else if (status == std::string("Busy")){
    ui->label->setStyleSheet("QLabel { background-color : blue; border-radius:20px;}");

} else if (status == std::string("Offline")){
    ui->label->setStyleSheet("QLabel { background-color : black; border-radius:20px;}");

}
}

void User::setName(std::string name) {
ui->label_2->setText(name.c_str());
}

std::string User::getName() {
    return ui->label_2->text().toStdString();
}

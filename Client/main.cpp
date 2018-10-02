#include "login.h"
#include "iostream"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Login w;
    std::cout << "qqsdqsd" << std::endl;
    w.show();

    return a.exec();
}

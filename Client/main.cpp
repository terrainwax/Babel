#include "login.h"
#include "iostream"
#include "client.h"
#include "GlobalSession.h"
#include <QApplication>

Client *client;

int main(int argc, char *argv[])
{

    //::ShowWindow( ::GetConsoleWindow(), SW_HIDE );
    QApplication a(argc, argv);
    client = new Client();
    if (argc == 2)
    client->serverName = argv[1];
    if (!client->Connect())
        exit(0);

    Login w;
    w.show();

    return a.exec();
}

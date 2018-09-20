//
// Created by entropy on 9/18/18.
//

#ifndef CPP_BABEL_2018_SERVER_H
#define CPP_BABEL_2018_SERVER_H

#include <list>
#include "User.h"

using boost::asio::ip::tcp;

class Server {
public:
    Server(unsigned short port);

    void run();

private:
    void startAccept();
    void handleAccept(Session::SessionPointer, const boost::system::error_code &error);

    boost::asio::io_context _io_context;
    tcp::endpoint _endpoint;
    tcp::acceptor _acceptor;
    User _user;
};

#endif //CPP_BABEL_2018_SERVER_H

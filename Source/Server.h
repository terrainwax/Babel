//
// Created by entropy on 9/18/18.
//

#ifndef CPP_BABEL_2018_SERVER_H
#define CPP_BABEL_2018_SERVER_H

#include <list>
#include "Room.h"
#include "User.h"

using boost::asio::ip::tcp;

class Server {
public:
    Server(boost::asio::io_service &io_service,
               const tcp::endpoint &endpoint);

private:
    void do_accept();

    tcp::acceptor _acceptor;
    tcp::socket _socket;
    Room _room;
};

#endif //CPP_BABEL_2018_SERVER_H

//
// Created by entropy on 9/18/18.
//

#ifndef CPP_BABEL_2018_SERVER_H
#define CPP_BABEL_2018_SERVER_H

#include <set>
#include <iostream>
#include <boost/asio/ip/tcp.hpp>

using boost::asio::ip::tcp;

class Session;
typedef boost::shared_ptr<Session> SessionPointer;

class Message;

class Server {
public:
    Server(unsigned short port);

    void run();
    void broadcast(Message message);

private:
    void startAccept();
    void handleAccept(SessionPointer, const boost::system::error_code &error);

    std::set<SessionPointer> _sessions;
    boost::asio::io_context _io_context;
    tcp::endpoint _endpoint;
    tcp::acceptor _acceptor;
};

#include "Session.h"
#include "Message.h"

#endif //CPP_BABEL_2018_SERVER_H

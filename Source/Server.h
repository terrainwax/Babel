//
// Created by entropy on 9/18/18.
//

#ifndef CPP_BABEL_2018_SERVER_H
#define CPP_BABEL_2018_SERVER_H

#include <set>
#include <iostream>
#include <boost/asio/ip/tcp.hpp>

using boost::asio::ip::tcp;

class ServerSession;
typedef boost::shared_ptr<ServerSession> ServerSessionPointer;

class User;
typedef boost::shared_ptr<User> UserPointer;

class Message;

class Server {
public:
    Server(unsigned short port);

    void run();
    void broadcast(Message message);
    User *getUser(const std::string &name);
    User *newUser(const std::string &name);


private:
    void startAccept();
    void handleAccept(ServerSessionPointer, const boost::system::error_code &error);

    std::set<ServerSessionPointer> _sessions;
    std::set<UserPointer> _users;
    boost::asio::io_context _io_context;
    tcp::endpoint _endpoint;
    tcp::acceptor _acceptor;
};

#include "ServerSession.h"
#include "User.h"
#include "Message.h"

#endif //CPP_BABEL_2018_SERVER_H

//
// Created by entropy on 9/18/18.
//

#ifndef CPP_BABEL_2018_CLIENT_H
#define CPP_BABEL_2018_CLIENT_H

#include <set>
#include <cstdlib>
#include <deque>
#include <thread>
#include <boost/asio.hpp>

#include "Logger.h"
#include "Packet.h"

using boost::asio::ip::tcp;

class ClientSession;
typedef boost::shared_ptr<ClientSession> ClientSessionPointer;

class Message;

class Client {
public:
    Client(const BabelString &address,
            unsigned short port);

    void start();
    void write(const BabelString &message);
    void display(Message message);

    void stop();

private:
    void startConnect();
    void handleConnect(ClientSessionPointer session,
                               const boost::system::error_code &error);

private:
    std::set<ClientSessionPointer> _sessions;
    boost::asio::io_context _io_context;
    tcp::endpoint _endpoint;
    std::thread _mainThread;
};

#include "ClientSession.h"
#include "Message.h"

#endif //CPP_BABEL_2018_CLIENT_H

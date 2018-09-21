//
// Created by entropy on 9/18/18.
//

#ifndef CPP_BABEL_2018_CLIENT_H
#define CPP_BABEL_2018_CLIENT_H

#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include "Packet.h"

using boost::asio::ip::tcp;

typedef std::deque<Packet> PacketQueue;

class Client {
public:
    Client(const std::string &username,
            const std::string &password,
            boost::asio::io_service &io_service,
               tcp::resolver::iterator endpoint_iterator);

    void write(const Packet &msg);

    void close();

private:
    void do_connect(tcp::resolver::iterator endpoint_iterator);

    void do_read_header();

    void do_read_body();

    void do_write();

private:
    std::string _username;
    std::string _password;
    boost::asio::io_service &_io_context;
    tcp::socket _socket;
    Packet _readMsg;
    PacketQueue _writeMsgQ;
};

#endif //CPP_BABEL_2018_CLIENT_H

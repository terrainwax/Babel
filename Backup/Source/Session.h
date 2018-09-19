//
// Created by entropy on 9/18/18.
//

#ifndef CPP_BABEL_2018_SESSION_H
#define CPP_BABEL_2018_SESSION_H

#include <boost/asio.hpp>
#include "Participant.h"
#include "Room.h"
#include "Packet.h"

using boost::asio::ip::tcp;

class Session
        : public Participant,
          public std::enable_shared_from_this<Session> {
public:
    Session(tcp::socket socket, Room &room);

    void start();

    void deliver(const Packet &msg);
    std::string getName();

private:
    void do_read_header();

    void do_read_body();

    void do_write();

    tcp::socket _socket;
    Room &_room;
    Packet _readMsg;
    Message_queue _writeMsgQ;
    std::string _name;
};

#endif //CPP_BABEL_2018_SESSION_H

//
// Created by entropy on 9/18/18.
//

#ifndef CPP_BABEL_2018_SESSION_H
#define CPP_BABEL_2018_SESSION_H

#include <boost/asio.hpp>
#include "Packet.h"

using boost::asio::ip::tcp;

class Room;
class Message;
typedef std::deque<Message*> MessageQueue;

class User {
public:
    User(tcp::socket socket);

    void start(Room *room);
    unsigned short getPort();
    std::string getAddress();

    void deliver(Message *message);
    std::string getName();

private:
    void do_read_header();

    void do_read_body();

    void do_write();

    tcp::socket _socket;
    unsigned short _port;
    boost::asio::ip::address _address;
    Packet _readPacket;
    MessageQueue _writeMessageQueue;
    std::string _name;
    Room *_room;
};

#include "Room.h"
#include "Message.h"

#endif //CPP_BABEL_2018_SESSION_H

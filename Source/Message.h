//
// Created by entropy on 9/20/18.
//

#ifndef CPP_BABEL_2018_MESSAGE_H
#define CPP_BABEL_2018_MESSAGE_H

#include "Packet.h"

class Session;

class Message {
public:
    Message(Packet packer);

    Packet getPacket() const;
    Session *getSender();

private:
    Packet _packet;
    Session *_sender;
};

std::ostream& operator<<(std::ostream& os, const Message& message);

typedef std::deque<Message> MessageQueue;

#include "Session.h"

#endif //CPP_BABEL_2018_MESSAGE_H

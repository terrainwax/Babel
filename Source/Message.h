//
// Created by entropy on 9/20/18.
//

#ifndef CPP_BABEL_2018_MESSAGE_H
#define CPP_BABEL_2018_MESSAGE_H

#include "Packet.h"

class ServerSession;

class Message {
public:
    Message(Packet packer, ServerSession *sender);

    Packet getPacket() const;
    ServerSession *getSender() const;
    bool hasSender() const;

private:
    Packet _packet;
    ServerSession *_sender;
};

std::ostream& operator<<(std::ostream& os, const Message& message);

typedef std::deque<Message> MessageQueue;

#include "ServerSession.h"

#endif //CPP_BABEL_2018_MESSAGE_H

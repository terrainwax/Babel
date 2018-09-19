//
// Created by entropy on 9/18/18.
//

#ifndef CPP_BABEL_2018_MESSAGE_H
#define CPP_BABEL_2018_MESSAGE_H

#include <string>
#include <deque>
#include "User.h"
#include "Packet.h"

class Message {

public:
    Message(User *sender, Packet packet);
    std::string getSender();
    std::string getContent();
    Packet getPacket();

private:
    User *_sender;
    Packet _packet;
    std::string _content;
};

typedef std::deque<Message*> MessageQueue;

#endif //CPP_BABEL_2018_MESSAGE_H

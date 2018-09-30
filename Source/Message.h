//
// Created by entropy on 9/20/18.
//

#ifndef CPP_BABEL_2018_MESSAGE_H
#define CPP_BABEL_2018_MESSAGE_H

#include <deque>
#include "BabelString.h"

class ServerSession;

class Message {
public:
    Message(BabelString content, ServerSession *sender);

    BabelString getContent() const;
    ServerSession *getSender() const;
    bool hasSender() const;

private:
    BabelString _content;
    ServerSession *_sender;
};

std::ostream& operator<<(std::ostream& os, const Message& message);

typedef std::deque<Message> MessageQueue;

#include "ServerSession.h"

#endif //CPP_BABEL_2018_MESSAGE_H

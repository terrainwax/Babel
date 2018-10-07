//
// Created by entropy on 9/20/18.
//

#ifndef CPP_BABEL_2018_MESSAGE_H
#define CPP_BABEL_2018_MESSAGE_H

#include <deque>
#include "BabelString.h"

class Message {
public:
    Message(BabelString content, Session *sender);

    BabelString getContent() const;
    Session *getSender() const;
    bool hasSender() const;

private:
    BabelString _content;
    Session *_sender;
};

std::ostream& operator<<(std::ostream& os, const Message& message);

typedef std::deque<Message> MessageQueue;


#endif //CPP_BABEL_2018_MESSAGE_H

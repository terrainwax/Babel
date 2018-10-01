//
// Created by entropy on 9/20/18.
//

#include "Message.h"


Message::Message(BabelString content, Session *sender)
    : _content(content), _sender(sender)
{

}

BabelString Message::getContent() const
{
    return _content;
}

bool Message::hasSender() const
{
    return _sender != nullptr;
}

Session *Message::getSender() const
{
    return _sender;
}

std::ostream& operator<<(std::ostream& os, const Message& message)
{
    os << "'" << message.getContent() << "'";

    if (message.hasSender())
        os << " from " << *message.getSender();

    return os;
}
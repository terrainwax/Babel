//
// Created by entropy on 9/20/18.
//

#include "Message.h"


Message::Message(Packet packet, ServerSession *sender)
    : _packet(packet), _sender(sender)
{

}

Packet Message::getPacket() const
{
    return _packet;
}

bool Message::hasSender() const
{
    return _sender != nullptr;
}

ServerSession *Message::getSender() const
{
    return _sender;
}

std::ostream& operator<<(std::ostream& os, const Message& message)
{
    os << "'" << std::string(message.getPacket().body(), message.getPacket().bodyLength()) << "'";

    if (message.hasSender())
        os << " from " << *message.getSender();

    return os;
}
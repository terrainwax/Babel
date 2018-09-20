//
// Created by entropy on 9/20/18.
//

#include "Message.h"


Message::Message(Packet packet)
    : _packet(packet), _sender(nullptr)
{

}

Packet Message::getPacket() const
{
    return _packet;
}

Session *Message::getSender()
{
    return _sender;
}

std::ostream& operator<<(std::ostream& os, const Message& message)
{
    os << std::string(message.getPacket().body(), message.getPacket().bodyLength());
    return os;
}
//
// Created by entropy on 9/18/18.
//

#include "Message.h"

Message::Message(User *sender, Packet packet)
    : _sender(sender), _packet(packet)
{
    _content = std::string(packet.body(), packet.bodyLength());
}

std::string Message::getSender()
{
    return _sender->getName();
}

std::string Message::getContent()
{
    return _content;
}

Packet Message::getPacket()
{
    return _packet;
}
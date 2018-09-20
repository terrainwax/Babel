//
// Created by entropy on 9/18/18.
//

#include "User.h"

User::User()
    : _name("Unknown")
{

}

void User::setName(const std::string &name)
{
    _name = name;
}

void User::addSession(SessionPointer session) {
    std::cout << "Session added." << std::endl;
    _sessions.insert(session);
    for (auto message: _recentMessageQueue)
        session->deliver(message.getPacket());
}

void User::removeSession(SessionPointer session) {
    std::cout << "Session removed." << std::endl;
    _sessions.erase(session);
}

void User::transmit(const Message &message) {
    _recentMessageQueue.push_back(message);

    while (_recentMessageQueue.size() > max_recent_messages)
        _recentMessageQueue.pop_front();

    for (auto session: _sessions)
        session->deliver(message.getPacket());
}
//
// Created by entropy on 9/18/18.
//

#include "Room.h"

void Room::join(User *user) {
    std::cout << "User joined room (" <<  std::to_string(_number) << "): " << user->getName() << std::endl;
    _members.insert(user);

    for (auto message: _recentMessageQueue)
        user->deliver(message);
}

void Room::leave(User *user) {
    std::cout << "User exited room (" <<  std::to_string(_number) << "): " << user->getName() << std::endl;
    _members.erase(user);
}

void Room::shout(Message *message) {
    std::cout << "[SHOUT] " << message->getSender() << ": " << message->getContent() << std::endl;
    _recentMessageQueue.push_back(message);

    while (_recentMessageQueue.size() > maximum_recent_messages)
        _recentMessageQueue.pop_front();

    for (auto member: _members)
        member->deliver(message);
}

void Room::whisper(Message *message, User *user)
{
    std::cout << "[WHISPER] " << message->getSender() << " -> " << user->getName() << ": " << message->getContent() << std::endl;

    user->deliver(message);
}

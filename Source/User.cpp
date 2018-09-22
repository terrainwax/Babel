//
// Created by entropy on 9/18/18.
//

#include "User.h"

User::User(Server &server, const std::string &name)
    : _server(server), _name(name), _password("password"), _isAvailable(true)
{

}

User::UserPointer User::create(Server &server, const std::string &name)
{
    return UserPointer(new User(server, name));
}

void User::setName(const std::string &name)
{
    std::cout << "User '" << _name << "' changed his name for '" << name << "'" << std::endl;

    _name = name;
}

std::string User::getName()
{
    return _name;
}

std::string User::getPassword()
{
    return _password;
}

void User::addSession(ServerSessionPointer session) {
    _sessions.insert(session);
    for (auto message: _recentMessageQueue)
        session->deliver(message.getPacket());
}

void User::removeSession(ServerSessionPointer session) {
    _sessions.erase(session);
}

void User::transmit(const Message &message) {
    _recentMessageQueue.push_back(message);

    while (_recentMessageQueue.size() > max_recent_messages)
        _recentMessageQueue.pop_front();

    for (auto session: _sessions)
        session->deliver(message.getPacket());
}

bool User::getStatus() const
{
    return _isAvailable;
}

void User::setStatus(bool status)
{
	_isAvailable = status;
}

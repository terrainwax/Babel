//
// Created by entropy on 9/18/18.
//

#include "User.h"

User::User(Server &server, const BabelString &name)
	: _server(server), _name(BabelString(name.getData())), _status(Status::AVAILABLE), _id(1)
{

}

User::UserPointer User::create(Server &server, const BabelString &name)
{
	return UserPointer(new User(server, name));
}

void User::setName(const BabelString &name)
{
	std::cout << "User '" << _name << "' changed his name for '" << name << "'" << std::endl;

	_name = name;
}

const BabelString &User::getPassword() const
{
	return _password;
}

void User::addSession(ServerSessionPointer session) {
	_sessions.insert(session);
	for (auto message: _recentMessageQueue)
		session->deliver(message.getContent());
}

void User::removeSession(ServerSessionPointer session) {
	_sessions.erase(session);
}

void User::transmit(const Message &message) {
	_recentMessageQueue.push_back(message);

	while (_recentMessageQueue.size() > max_recent_messages)
		_recentMessageQueue.pop_front();

	for (auto session: _sessions)
		session->deliver(message.getContent());
}

User::Status User::getStatus() const
{
	return _status;
}

void User::setStatus(User::Status status)
{
	_status = status;
}

unsigned char User::getID() const
{
	return _id;
}

void User::setID(unsigned char id)
{
	_id = id;
}

void User::setPassword(const BabelString &password)
{
	_password = password;
}

const BabelString &User::getName() const
{
	return _name;
}

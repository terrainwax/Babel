//
// Created by entropy on 9/18/18.
//

#include "User.h"

User::User(Server &server, const BabelString &name)
	: _server(server), _name(BabelString(name.getData())), _status(Status::AVAILABLE), _call(nullptr)
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
	for (const auto &message: _recentMessageQueue)
		session->deliver(message.getContent());
}

void User::removeSession(ServerSessionPointer session) {
	_sessions.erase(session);
}

void User::transmit(const Message &message) {
	_recentMessageQueue.push_back(message);

	while (_recentMessageQueue.size() > max_recent_messages)
		_recentMessageQueue.pop_front();

	for (const auto &session: _sessions)
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

void User::setPassword(const BabelString &password)
{
	_password = password;
}

const BabelString &User::getName() const
{
	return _name;
}

void User::hostCall(BabelString ip, BabelString port)
{
	_call = new Call(this, ip, port);
}

Call *User::getCall()
{
	return _call;
}

void User::stopCall()
{
	_status = Status::AVAILABLE;
	delete _call;
	_call = nullptr;
}

ServerSessionPointer User::getRecentSession()
{
	ServerSessionPointer ptr = *_sessions.begin();
	for (const auto &pointer : _sessions) {
		if (pointer->isActive())
			ptr = pointer;
	}

	return (ptr);
}


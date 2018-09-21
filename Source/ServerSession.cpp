/*
** EPITECH PROJECT, 2018
** CPP_babel_2018
** File description:
** ServerSession.cpp
*/

#include <iostream>
#include "ServerSession.h"

ServerSession::ServerSession(Server &server, boost::asio::io_context &io_context) : _server(server),_socket(io_context), _user(nullptr)
{

}

ServerSession::SessionPointer ServerSession::create(Server &server, boost::asio::io_context &io_context)
{
	return SessionPointer(new ServerSession(server, io_context));
}

tcp::socket &ServerSession::getSocket()
{
	return _socket;
}

std::string ServerSession::getAddress() const
{
	return _socket.remote_endpoint().address().to_string();
}

void ServerSession::open() {
	std::cout << "ServerSession opened: " << getAddress() << std::endl;
	startReadHeader();
}

bool ServerSession::hasUser() const
{
	return _user != nullptr;
}

void ServerSession::setUser(User *user)
{
	std::cout << "ServerSession from " << getAddress() << " now belongs to user " << user->getName() << std::endl;
	_user = user;
	_user->addSession(shared_from_this());
}

User *ServerSession::getUser() const
{
	return _user;
}

void ServerSession::startReadHeader()
{
	boost::asio::async_read(_socket, boost::asio::buffer(_readMsg.data(), Packet::header_length),
							boost::bind(&ServerSession::handleReadHeader, shared_from_this(),
										boost::asio::placeholders::error,
										boost::asio::placeholders::bytes_transferred));
}

void ServerSession::handleReadHeader(const boost::system::error_code &error, size_t bytes)
{
	if (!error && _readMsg.decodeHeader()) {
		startReadBody();
	} else {
		if (hasUser())
			_user->removeSession(shared_from_this());
	}
}

void ServerSession::startReadBody()
{
	boost::asio::async_read(_socket, boost::asio::buffer(_readMsg.body(), _readMsg.bodyLength()),
							 boost::bind(&ServerSession::handleReadBody, shared_from_this(),
										 boost::asio::placeholders::error,
										 boost::asio::placeholders::bytes_transferred));
}

void ServerSession::handleReadBody(const boost::system::error_code &error, size_t bytes)
{
	if (!error) {

		if (_readMsg.str().substr(0, 6) == "LOGIN:") {

			std::string substr = _readMsg.str().substr(6, _readMsg.bodyLength() - 6);
			std::vector<std::string> split;
			boost::split(split, substr, [](char c){return c == '/';});
			std::string username = split[0];

			if (hasUser())
				_user->setName(username);
			else {
				User *user = _server.getUser(username);
				if (_server.getUser(username) == nullptr)
					setUser(_server.newUser(username));
				else
					{
					std::string password = split[1];
					if (user->getPassword() == password)
						setUser(user);
					else
						std::cout << "ServerSession " << *this << " failed login with username " << username << " and password " << password << std::endl;
				}
			}
		}
		else {
			_server.broadcast(Message(_readMsg, this));
		}
		startReadHeader();
	} else {
		if (hasUser())
			_user->removeSession(shared_from_this());
	}
}

void ServerSession::startWrite()
{
	auto _message = std::string(_writeMessageQueue.front().data(),
								_writeMessageQueue.front().length());

	boost::asio::async_write(_socket, boost::asio::buffer(_message),
		boost::bind(&ServerSession::handleWrite, shared_from_this(),
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
}

void ServerSession::handleWrite(const boost::system::error_code &error, size_t bytes)
{
	if (!error) {
		_writeMessageQueue.pop_front();
		if (!_writeMessageQueue.empty()) {
			startWrite();
		}
	} else {
		if (hasUser())
			_user->removeSession(shared_from_this());
	}
}

void ServerSession::deliver(const Packet &msg)
{
	bool write_in_progress = !_writeMessageQueue.empty();
	_writeMessageQueue.push_back(msg);

	if (!write_in_progress) {
		startWrite();
	}
}

std::ostream& operator<<(std::ostream& os, const ServerSession& session)
{
	os << session.getAddress();

	if (session.hasUser())
		os << "(" << session.getUser()->getName() << ")";
	else
		os << "(Unknown)";

	return os;
}
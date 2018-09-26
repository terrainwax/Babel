/*
** EPITECH PROJECT, 2018
** CPP_babel_2018
** File description:
** ServerSession.cpp
*/

#include <iostream>
#include "ServerSession.h"

ServerSession::ServerSession(Server &server, boost::asio::io_context &io_context) : _server(server),_socket(io_context), _user(nullptr), _crypto()
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
	sendRSAPublicKey();
	//receiveAESKey();
	startReadHeader();
}

void ServerSession::sendRSAPublicKey()
{
	std::cout << "Sending RSA Public Key." << std::endl;

	std::string key = _crypto.getLocalPublicKey();

	std::cout << "'" << key << "'" << std::endl;

	Packet packet;
	packet.bodyLength(key.size());
	std::memcpy(packet.body(), key.data(), packet.bodyLength());
	packet.encodeHeader();
	deliver(packet);
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
		_server.getLexer().parse(_readMsg, this);
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

void ServerSession::deliverString(std::string string)
{
	Packet packet;

	packet.bodyLength(string.size());
	std::strcpy(packet.body(), string.c_str());
	packet.encodeHeader();
	deliver(packet);
}
/*
** EPITECH PROJECT, 2018
** CPP_babel_2018
** File description:
** Session.cpp
*/

#include <iostream>
#include "Session.h"

Session::Session(Server &server, boost::asio::io_context &io_context) : _server(server),_socket(io_context), _user(nullptr)
{

}

Session::SessionPointer Session::create(Server &server, boost::asio::io_context &io_context)
{
	return SessionPointer(new Session(server, io_context));
}

tcp::socket &Session::getSocket()
{
	return _socket;
}

std::string Session::getAddress()
{
	return _socket.remote_endpoint().address().to_string();
}

void Session::start() {
	startReadHeader();
}

bool Session::hasUser()
{
	return _user != nullptr;
}

void Session::setUser(User *user)
{
	_user = user;
	_user->addSession(shared_from_this());
}

void Session::startReadHeader()
{
	boost::asio::async_read(_socket, boost::asio::buffer(_readMsg.data(), Packet::header_length),
							boost::bind(&Session::handleReadHeader, shared_from_this(),
										boost::asio::placeholders::error,
										boost::asio::placeholders::bytes_transferred));
}

void Session::handleReadHeader(const boost::system::error_code &error, size_t bytes)
{
	if (!error && _readMsg.decodeHeader()) {
		startReadBody();
	} else {
		if (hasUser())
			_user->removeSession(shared_from_this());
	}
}

void Session::startReadBody()
{
	boost::asio::async_read(_socket, boost::asio::buffer(_readMsg.body(), _readMsg.bodyLength()),
							 boost::bind(&Session::handleReadBody, shared_from_this(),
										 boost::asio::placeholders::error,
										 boost::asio::placeholders::bytes_transferred));
}

void Session::handleReadBody(const boost::system::error_code &error, size_t bytes)
{
	if (!error) {
		if (hasUser())
			_user->transmit(Message(_readMsg));
		else
			_server.broadcast(Message(_readMsg));
		//if (_readMsg.str().substr(0, 5) == "NAME:")
			//_user.setName(_readMsg.str().substr(5, _readMsg.bodyLength() - 5));
		//else
		startReadHeader();
	} else {
		if (hasUser())
			_user->removeSession(shared_from_this());
	}
}

void Session::startWrite()
{
	auto _message = std::string(_writeMessageQueue.front().data(),
								_writeMessageQueue.front().length());

	boost::asio::async_write(_socket, boost::asio::buffer(_message),
		boost::bind(&Session::handleWrite, shared_from_this(),
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
}

void Session::handleWrite(const boost::system::error_code &error, size_t bytes)
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

void Session::deliver(const Packet &msg)
{
	bool write_in_progress = !_writeMessageQueue.empty();
	_writeMessageQueue.push_back(msg);

	if (!write_in_progress) {
		startWrite();
	}
}
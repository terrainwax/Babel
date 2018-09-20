/*
** EPITECH PROJECT, 2018
** CPP_babel_2018
** File description:
** Session.cpp
*/

#include <iostream>
#include "Session.h"

Session::Session(boost::asio::io_context &io_context, User &user) : _socket(io_context), _user(user)
{

}

Session::SessionPointer Session::create(boost::asio::io_context &io_context, User &user)
{
	return SessionPointer(new Session(io_context, user));
}

tcp::socket &Session::getSocket()
{
	return _socket;
}

void Session::start() {
	_user.addSession(shared_from_this());
	startReadHeader();
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
		_user.removeSession(shared_from_this());
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
		_user.transmit(shared_from_this(), _readMsg);
		//if (_readMsg.str().substr(0, 5) == "NAME:")
			//_name = _readMsg.str().substr(5, _readMsg.bodyLength() - 5);
		startReadHeader();
	} else {
		_user.removeSession(shared_from_this());
	}
}

void Session::startWrite()
{
	auto _message = std::string(_writeMsgQ.front().data(),
								_writeMsgQ.front().length());

	boost::asio::async_write(_socket, boost::asio::buffer(_message),
		boost::bind(&Session::handleWrite, shared_from_this(),
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
}

void Session::handleWrite(const boost::system::error_code &error, size_t bytes)
{
	std::cout << "Written to someone" << std::endl;

	if (!error) {
		_writeMsgQ.pop_front();
		if (!_writeMsgQ.empty()) {
			startWrite();
		}
	} else {
		_user.removeSession(shared_from_this());
	}
}

void Session::deliver(const Packet &msg)
{
	bool write_in_progress = !_writeMsgQ.empty();
	_writeMsgQ.push_back(msg);

	if (!write_in_progress) {
		startWrite();
	}
}
/*
** EPITECH PROJECT, 2018
** CPP_babel_2018
** File description:
** Session.hpp
*/

#ifndef CPP_BABEL_2018_CONNECTION_HPP
#define CPP_BABEL_2018_CONNECTION_HPP

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "Packet.h"

using boost::asio::ip::tcp;
class User;

class Session : public boost::enable_shared_from_this<Session>
{
public:
	typedef boost::shared_ptr<Session> SessionPointer;

	static SessionPointer create(boost::asio::io_context& io_context, User &user);
	tcp::socket& getSocket();
	void start();
	void deliver(const Packet &msg);

private:

	explicit Session(boost::asio::io_context& io_context, User &user);
	void startWrite();
	void handleWrite(const boost::system::error_code &error, size_t bytes);
	void startReadHeader();
	void handleReadHeader(const boost::system::error_code &error, size_t bytes);
	void startReadBody();
	void handleReadBody(const boost::system::error_code &error, size_t bytes);

	tcp::socket _socket;
	User &_user;
	Packet _readMsg;
	Message_queue _writeMsgQ;
};

#include "User.h"

#endif //CPP_BABEL_2018_CONNECTION_HPP

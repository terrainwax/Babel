/*
** EPITECH PROJECT, 2018
** CPP_babel_2018
** File description:
** Session.hpp
*/

#ifndef CPP_BABEL_2018_CONNECTION_HPP
#define CPP_BABEL_2018_CONNECTION_HPP

#include <deque>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/algorithm/string.hpp>
#include "Packet.h"

using boost::asio::ip::tcp;

class Server;
class User;

class Session : public boost::enable_shared_from_this<Session>
{
public:
	typedef boost::shared_ptr<Session> SessionPointer;

	static SessionPointer create(Server &_server, boost::asio::io_context& io_context);
	tcp::socket& getSocket();
	void start();
	void deliver(const Packet &msg);
	bool hasUser() const;
	void setUser(User *user);
	User *getUser() const;
	std::string getAddress() const;

private:

	explicit Session(Server &_server, boost::asio::io_context& io_context);
	void startWrite();
	void handleWrite(const boost::system::error_code &error, size_t bytes);
	void startReadHeader();
	void handleReadHeader(const boost::system::error_code &error, size_t bytes);
	void startReadBody();
	void handleReadBody(const boost::system::error_code &error, size_t bytes);

	Server &_server;
	tcp::socket _socket;
	Packet _readMsg;
	PacketQueue _writeMessageQueue;
	User *_user;
};

std::ostream& operator<<(std::ostream& os, const Session& session);

#include "Server.h"
#include "User.h"

#endif //CPP_BABEL_2018_CONNECTION_HPP

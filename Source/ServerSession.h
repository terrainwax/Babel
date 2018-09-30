/*
** EPITECH PROJECT, 2018
** CPP_babel_2018
** File description:
** ServerSession.hpp
*/

#ifndef CPP_BABEL_2018_SERVER_SESSION_HPP
#define CPP_BABEL_2018_SERVER_SESSION_HPP

#include <deque>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/algorithm/string.hpp>

#include "BabelString.h"
#include "Packet.h"
#include "ServerCrypto.h"

using boost::asio::ip::tcp;

class Server;
class User;

class ServerSession : public boost::enable_shared_from_this<ServerSession>
{
public:
	typedef boost::shared_ptr<ServerSession> SessionPointer;

	static SessionPointer create(Server &_server, boost::asio::io_context& io_context);
	tcp::socket& getSocket();
	void open();
	void deliver(const BabelString &message);
	bool hasUser() const;
	void setUser(User *user);
	User *getUser() const;
	BabelString getAddress() const;

private:

	explicit ServerSession(Server &_server, boost::asio::io_context& io_context);
	void sendRSAPublicKey();
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
	ServerCrypto _crypto;
	bool _secured;
};

std::ostream& operator<<(std::ostream& os, const ServerSession& session);

#include "Server.h"
#include "User.h"

#endif //CPP_BABEL_2018_SERVER_SESSION_HPP

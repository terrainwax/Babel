/*
** EPITECH PROJECT, 2018
** CPP_babel_2018
** File description:
** ServerSession.hpp
*/

#ifndef CPP_BABEL_2018_SERVER_SESSION_HPP
#define CPP_BABEL_2018_SERVER_SESSION_HPP

#include <deque>
#include <iomanip>

#include "Session.h"
#include "BabelString.h"
#include "Packet.h"
#include "ServerCrypto.h"

using boost::asio::ip::tcp;

class Server;
class User;

class ServerSession : public boost::enable_shared_from_this<ServerSession>, public Session
{
public:
	typedef boost::shared_ptr<ServerSession> SessionPointer;

	static SessionPointer create(Server &_server, boost::asio::io_context& io_context);
	void open() override;
	void close() override;
	void deliver(const BabelString &message);
	bool hasUser() const;
	void setUser(User *user);
	User *getUser() const;
	void setupCrypto();

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
	Packet _readMsg;
	PacketQueue _writeMessageQueue;
	User *_user;
	ServerCrypto _crypto;
};

#include "Server.h"
#include "User.h"

#endif //CPP_BABEL_2018_SERVER_SESSION_HPP

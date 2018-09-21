/*
** EPITECH PROJECT, 2018
** CPP_babel_2018
** File description:
** ServerSession.hpp
*/

#ifndef CPP_BABEL_2018_SERVER_SESSION_HPP
#define CPP_BABEL_2018_SERVER_SESSION_HPP

#include <deque>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/algorithm/string.hpp>
#include <openssl/rand.h>
#include <openssl/evp.h>
#include <openssl/engine.h>
#include <openssl/pem.h>

#include "Packet.h"


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
	void deliver(const Packet &msg);
	bool hasUser() const;
	void setUser(User *user);
	User *getUser() const;
	std::string getAddress() const;

private:

	explicit ServerSession(Server &_server, boost::asio::io_context& io_context);
	void generateKeyPair();
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
	RSA *_keypair;
	std::string _publicKey;
	std::string _privateKey;
};

std::ostream& operator<<(std::ostream& os, const ServerSession& session);

#include "Server.h"
#include "User.h"

#endif //CPP_BABEL_2018_SERVER_SESSION_HPP

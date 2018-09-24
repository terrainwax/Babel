/*
** EPITECH PROJECT, 2018
** CPP_babel_2018
** File description:
** ClientSession.hpp
*/

#ifndef CPP_BABEL_2018_CLIENT_SESSION_HPP
#define CPP_BABEL_2018_CLIENT_SESSION_HPP

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

class Client;

class ClientSession : public boost::enable_shared_from_this<ClientSession>
{
public:
	typedef boost::shared_ptr<ClientSession> SessionPointer;

	static SessionPointer create(Client &_client, boost::asio::io_context& io_context);
	tcp::socket& getSocket();
	void open();
	void deliver(const Packet &msg);
	std::string getAddress() const;

private:

	explicit ClientSession(Client &_client, boost::asio::io_context& io_context);
	void startWrite();
	void handleWrite(const boost::system::error_code &error, size_t bytes);
	void startReadHeader();
	void handleReadHeader(const boost::system::error_code &error, size_t bytes);
	void startReadBody();
	void handleReadBody(const boost::system::error_code &error, size_t bytes);

	Client &_client;
	tcp::socket _socket;
	Packet _readMsg;
	PacketQueue _writeMessageQueue;
	RSA *_public_key;
};

std::ostream& operator<<(std::ostream& os, const ClientSession& session);

#include "Client.h"
#include "User.h"

#endif //CPP_BABEL_2018_CLIENT_SESSION_HPP

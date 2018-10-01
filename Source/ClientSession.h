/*
** EPITECH PROJECT, 2018
** CPP_babel_2018
** File description:
** ClientSession.hpp
*/

#ifndef CPP_BABEL_2018_CLIENT_SESSION_HPP
#define CPP_BABEL_2018_CLIENT_SESSION_HPP

#include <deque>
#include <iomanip>

#include "Session.h"
#include "BabelString.h"
#include "Packet.h"
#include "ClientCrypto.h"

using boost::asio::ip::tcp;

class Client;

class ClientSession : public boost::enable_shared_from_this<ClientSession>, public Session
{
public:
	typedef boost::shared_ptr<ClientSession> SessionPointer;

	static SessionPointer create(Client &_client, boost::asio::io_context& io_context);
	void open();
	void close();
	void deliver(const BabelString &message);

private:

	explicit ClientSession(Client &_client, boost::asio::io_context& io_context);
	void startWrite();
	void handleWrite(const boost::system::error_code &error, size_t bytes);
	void startReadHeader();
	void handleReadHeader(const boost::system::error_code &error, size_t bytes);
	void startReadBody();
	void handleReadBody(const boost::system::error_code &error, size_t bytes);

	Client &_client;
	Packet _readMsg;
	PacketQueue _writeMessageQueue;
	ClientCrypto _crypto;
};

#include "Client.h"
#include "User.h"

#endif //CPP_BABEL_2018_CLIENT_SESSION_HPP

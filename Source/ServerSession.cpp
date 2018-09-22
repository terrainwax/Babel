/*
** EPITECH PROJECT, 2018
** CPP_babel_2018
** File description:
** ServerSession.cpp
*/

#include <iostream>
#include "ServerSession.h"

ServerSession::ServerSession(Server &server, boost::asio::io_context &io_context) : _server(server),_socket(io_context), _user(nullptr)
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
	generateKeyPair();
	sendRSAPublicKey();
	//receiveAESKey();
	startReadHeader();
}

void ServerSession::generateKeyPair()
{
	std::cout << "Generating RSA KeyPair" << std::endl;

	RAND_load_file("/dev/urandom", 1024);

	_keypair = RSA_generate_key(2048, RSA_F4, NULL, NULL);

	if (RSA_check_key(_keypair) != 1)
		std::cout << "Error occured while generating RSA keypair" << std::endl;

	BIO *privateBIO = BIO_new(BIO_s_mem());
	BIO *publicBIO = BIO_new(BIO_s_mem());

	PEM_write_bio_RSAPrivateKey(privateBIO, _keypair, nullptr, nullptr, 0, nullptr, nullptr);
	PEM_write_bio_RSAPublicKey(publicBIO, _keypair);

	int pri_len = BIO_pending(privateBIO);
	int pub_len = BIO_pending(publicBIO);

	_privateKey = std::string(pri_len, '\0');
	_publicKey = std::string(pub_len, '\0');

	BIO_read(privateBIO, _privateKey.data(), pri_len);
	BIO_read(publicBIO, _publicKey.data(), pub_len);
	
	_privateKey = _privateKey.substr(0, _privateKey.size() - 1);
	_publicKey = _publicKey.substr(0, _publicKey.size() - 1);

	BIO_free_all(publicBIO);
	BIO_free_all(privateBIO);
}

void ServerSession::sendRSAPublicKey()
{
	std::cout << "Sending RSA Public Key." << std::endl;

	Packet packet;
	packet.bodyLength(_publicKey.size());
	std::memcpy(packet.body(), _publicKey.data(), packet.bodyLength());
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

		if (_readMsg.str().substr(0, 6) == "LOGIN:") {

			std::string substr = _readMsg.str().substr(6, _readMsg.bodyLength() - 6);
			std::vector<std::string> split;
			boost::split(split, substr, [](char c){return c == '/';});
			std::string username = split[0];

			if (hasUser())
				_user->setName(username);
			else {
				User *user = _server.getUser(username);
				if (_server.getUser(username) == nullptr)
					setUser(_server.newUser(username));
				else
					{
					std::string password = split[1];
					if (user->getPassword() == password)
						setUser(user);
					else
						std::cout << "ServerSession " << *this << " failed login with username " << username << " and password " << password << std::endl;
				}
			}
		}
		else {
			_server.getLexer().parse(_readMsg, this);
		}
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
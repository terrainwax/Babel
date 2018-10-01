/*
** EPITECH PROJECT, 2018
** CPP_babel_2018
** File description:
** ServerSession.cpp
*/

#include <iostream>
#include "ServerSession.h"

ServerSession::ServerSession(Server &server, boost::asio::io_context &io_context) : _server(server),_socket(io_context), _user(nullptr), _crypto(), _secured(false)
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

BabelString ServerSession::getAddress() const
{
	return BabelString(_socket.remote_endpoint().address().to_string().c_str());
}

void ServerSession::open() {
    Logger::get()->debug(BabelString("ServerSession Opened: ") + getAddress());
	sendRSAPublicKey();
	startReadHeader();
}

void ServerSession::close() {
	Logger::get()->debug(BabelString("ServerSession Closed: ") + getAddress());
	_socket.close();
}

void ServerSession::sendRSAPublicKey()
{
	BabelString key = _crypto.getLocalPublicKey();

    std::cout << "Sending RSA Public Key:" << std::endl << key << std::endl;

	deliver(key);
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
		if (!_secured) {
			if (_readMsg.str().substr(0, 18) == "ENCRYPTED AES KEY:") {
				BabelString encryptedAESKey = _readMsg.str().substr(18, _readMsg.str().getSize() - 18);

				_crypto.setAESKey(_crypto.decryptRSA(encryptedAESKey));

				BabelString aesKey = _crypto.getAESKey();

				std::cout << "Received Encrypted AES Key: ";

				for (int i = 0; i < aesKey.getSize(); ++i)
					std::cout << std::hex << std::setfill('0') << std::setw(2) << (unsigned int)(unsigned char)aesKey.getData()[i];

				std::cout << std::endl;
			}
			if (_readMsg.str().substr(0, 17) == "ENCRYPTED AES IV:") {
				BabelString encryptedAESIv = _readMsg.str().substr(17, _readMsg.str().getSize() - 17);

				_crypto.setAESIv(_crypto.decryptRSA(encryptedAESIv));

				BabelString aesIv = _crypto.getAESIv();

				std::cout << "Received Encrypted AES Iv: ";

				for (int i = 0; i < aesIv.getSize(); ++i)
					std::cout << std::hex << std::setfill('0') << std::setw(2) << (unsigned int)(unsigned char)aesIv.getData()[i];

				std::cout << std::endl;

                Logger::get()->debug(BabelString("ServerSession Secured: ") + getAddress());

				_secured = true;
			}
		}
        else
		{
        	BabelString decrypted = _crypto.decryptAES(_readMsg.str());
			_server.getLexer().parse(decrypted, this);
		}
		startReadHeader();
	} else {
		if (hasUser())
			_user->removeSession(shared_from_this());
	}
}

void ServerSession::startWrite()
{
	auto _message = BabelString(_writeMessageQueue.front().data(),
		_writeMessageQueue.front().length());

	boost::asio::async_write(_socket, boost::asio::buffer(_message.getData(), _message.getSize()),
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

std::ostream& operator<<(std::ostream& os, const ServerSession& session)
{
	os << session.getAddress();

	if (session.hasUser())
		os << "(" << session.getUser()->getName() << ")";
	else
		os << "(Unknown)";

	return os;
}

void ServerSession::deliver(const BabelString &message)
{
	BabelString data = message;

	if (_secured)
		data = _crypto.encryptAES(message);

	Packet packet;
	packet.bodyLength(data.getSize());
	std::memcpy(packet.body(), data.getData(), packet.bodyLength());
	packet.encodeHeader();

	bool write_in_progress = !_writeMessageQueue.empty();
	_writeMessageQueue.push_back(packet);

	if (!write_in_progress) {
		startWrite();
	}
}
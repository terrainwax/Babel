/*
** EPITECH PROJECT, 2018
** CPP_babel_2018
** File description:
** ServerSession.cpp
*/

#include <iostream>
#include "ServerSession.h"

ServerSession::ServerSession(Server &server, boost::asio::io_context &io_context) : Session(io_context), _server(server), _user(nullptr), _crypto()
{

}

ServerSession::SessionPointer ServerSession::create(Server &server, boost::asio::io_context &io_context)
{
	return SessionPointer(new ServerSession(server, io_context));
}

void ServerSession::open() {
	if (isActive())
		throw ServerException("Cannot Open Active ServerSession");

	Session::open();
	Logger::get()->debug(BabelString("ServerSession Opened: ") + getAddress());
	sendRSAPublicKey();
	startReadHeader();
}

void ServerSession::close() {
	if (!isActive())
		throw ServerException("Cannot Open Inactive ClientSession");

	if (hasUser()) {
		for (auto it = _server.getOnlineUsers().begin(); it != _server.getOnlineUsers().end(); it++) {
			if (*it == _user) {
				_server.broadcast()
				_server.getOnlineUsers().erase(it);
				break;
			}
		}
		_user->removeSession(shared_from_this());
	}
	Logger::get()->debug(BabelString("ServerSession Closed From Someone. We Don't Know. We Don't Care."));
	Session::close();
}

void ServerSession::sendRSAPublicKey()
{
	BabelString key = _crypto.getLocalPublicKey();

	Logger::get()->debug(BabelString("Sending RSA Public Key:\n") + key);

	deliver(key);
}

bool ServerSession::hasUser() const
{
	return _user != nullptr;
}

void ServerSession::setUser(User *user)
{
	Logger::get()->debug(BabelString("ServerSession ") + getAddress() + BabelString(" Assigned To ") + user->getName());
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
		close();
	}
}

void ServerSession::setupCrypto()
{
	if (_readMsg.str().substr(0, 18) == "ENCRYPTED AES KEY:") {
		BabelString encryptedAESKey = _readMsg.str().substr(18, _readMsg.str().getSize() - 18);

		_crypto.setAESKey(_crypto.decryptRSA(encryptedAESKey));

		BabelString aesKey = _crypto.getAESKey();

		std::cout << "Received Encrypted AES Key: ";

		for (int i = 0; i < aesKey.getSize(); ++i)
			std::cout << std::hex << std::setfill('0') << std::setw(2) << (unsigned int)(unsigned char)aesKey.getData()[i];

		std::cout << std::endl;
	}
	else if (_readMsg.str().substr(0, 17) == "ENCRYPTED AES IV:") {
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
	else
	{
		Logger::get()->error("ServerSession Could Not Be Secured");
		close();
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
			setupCrypto();
		}
		else
		{
			BabelString decrypted = _crypto.decryptAES(_readMsg.str());
			_server.getLexer().parse(decrypted, this);
		}
		startReadHeader();
	} else {
		close();
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
		close();
	}
}

void ServerSession::deliver(const BabelString &message)
{
	BabelString data = message;

	if (isSecured())
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
//
// Created by entropy on 9/18/18.
//

#include "Client.h"

Client::Client(const BabelString &address,
	unsigned short port)
	: _io_context(),
	_endpoint(boost::asio::ip::address::from_string(address.getData()), port)
{

	startConnect();
}

void Client::start()
{
	std::cout << "Client started." << std::endl;
	_mainThread = std::thread([this]() {
		try {
			_io_context.run();
		} catch (std::exception &e) {
			std::cout << std::endl << "An error occurred." << std::endl;
			exit(1);
		}
	});
}

void Client::startConnect() {

	ClientSession::SessionPointer session = ClientSession::create(*this, _io_context);

	boost::asio::async_connect(session->getSocket(), &_endpoint,
		boost::bind(&Client::handleConnect, this, session,
			boost::asio::placeholders::error));
}

void Client::handleConnect(ClientSession::SessionPointer session,
	const boost::system::error_code &error)
{
	if (!error) {
		_sessions.insert(session);
		session->open();
	}
}

void Client::display(Message message)
{
	std::cout << "Received message: " << message << std::endl;
}

void Client::write(const BabelString &message) {
	for (auto session: _sessions)
		session->deliver(message);
}

void Client::stop() {
	//_io_context.post([this]() { _socket.stop(); });
	_mainThread.join();
}



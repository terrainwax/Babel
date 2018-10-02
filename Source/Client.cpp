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
    Logger::get()->debug("Client Started");
	_mainThread = std::thread([this]() {
		try {
			_io_context.run();
		} catch (std::exception &e) {
			throw ClientException("IOContext Error In Client");
		}
	});
}

void Client::startConnect() {

	if (!_io_context.stopped()) {
		ClientSession::SessionPointer session = ClientSession::create(*this, _io_context);

		boost::asio::async_connect(session->getSocket(), &_endpoint,
								   boost::bind(&Client::handleConnect, this, session,
											   boost::asio::placeholders::error));
	}
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
	Logger::get()->debug(BabelString("Received Message: '") + message.getContent() + BabelString("' From ") + message.getSender()->getAddress());
}

void Client::write(const BabelString &message) {
	for (auto session: _sessions)
		session->deliver(message);
}

void Client::stop() {
	for (auto session: _sessions)
		session->close();

	_io_context.stop();
	_mainThread.join();

	Logger::get()->debug("Client Stopped");
}



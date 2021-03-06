//
// Created by entropy on 9/18/18.
//

#include "Server.h"

Server::Server(unsigned short port)
	: _io_context(), _endpoint(tcp::endpoint(tcp::v4(), port)),
	_acceptor(_io_context, _endpoint), _lexer(*this), _active(false)
{
	startAccept();
}

void Server::start()
{
	_active = true;
	Logger::get()->debug("Server Started");
	_mainThread = std::thread([this]() {
		while (_active) {
			try {
				_io_context.run();
			} catch (std::exception &e) {
			}
		}
	});
}

void Server::stop()
{
	_active = false;

	for (const auto &session: _sessions) {
		if (session->isActive())
			session->close();
	}

	_io_context.stop();
	_mainThread.join();

	Logger::get()->debug("Server Stopped");
}

void Server::broadcast(const BabelString &message)
{
	Logger::get()->debug(
		BabelString("Broadcasting Message: '") + message + "'");

	for (const auto &session: _sessions) {
		if (session->isActive())
			session->deliver(message);
	}
}

User *Server::getUser(const BabelString &name)
{
	for (auto user: _users) {
		if (user->getName() == name)
			return user.get();
	}

	return nullptr;
}

User *Server::newUser(const BabelString &name)
{
	Logger::get()->debug(BabelString("New User Created: ") + name);

	UserPointer user = User::create(*this, name);
	_users.insert(user);

	return user.get();
}

void Server::startAccept()
{
	if (!_io_context.stopped()) {
		ServerSession::SessionPointer session = ServerSession::create(
			*this, _acceptor.get_executor().context());

		_acceptor.async_accept(session->getSocket(),
			boost::bind(&Server::handleAccept, this, session,
				boost::asio::placeholders::error));
	}
}

void Server::handleAccept(ServerSession::SessionPointer session,
	const boost::system::error_code &error
)
{
	if (!error) {
		_sessions.insert(session);
		session->open();
	}

	startAccept();
}

CommandLexer &Server::getLexer()
{
	return _lexer;
}

std::vector<User *> &Server::getOnlineUsers()
{
	return _onlineUsers;
}

Server::~Server()
{
	stop();
}

void Server::broadcast(const BabelString &message, ServerSession *sessionBroadcaster)
{
	Logger::get()->debug(
		BabelString("Broadcasting Message: '") + message + "'");

	for (const auto &session : _sessions) {
		if (session->hasUser() && session->getUser() != sessionBroadcaster->getUser() && session->isActive())
			session->deliver(message);
	}
}

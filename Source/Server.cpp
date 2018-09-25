//
// Created by entropy on 9/18/18.
//

#include "Server.h"

Server::Server(unsigned short port) : _io_context(), _endpoint(tcp::endpoint(tcp::v4(), port)),
_acceptor(_io_context, _endpoint), _lexer(*this)
{
    startAccept();
}

void Server::start()
{
    std::cout << "Server started." << std::endl;
    _mainThread = std::thread([this]() { _io_context.run(); });
}

void Server::stop() {
    //_io_context.post([this]() { _socket.stop(); });
    _mainThread.join();
}

void Server::broadcast(Message message)
{
    std::cout << "Broadcasting message: " << message << std::endl;

    for (auto session: _sessions)
        session->deliver(message.getPacket());
}

User *Server::getUser(const std::string &name) {
    for (auto user: _users)
    {
        if (user->getName() == name)
            return user.get();
    }

    return nullptr;
}

User *Server::newUser(const std::string &name) {
    std::cout << "New user : " << name << std::endl;

    UserPointer user = User::create(*this, name);
    _users.insert(user);

    return user.get();
}

void Server::startAccept()
{
    ServerSession::SessionPointer session =
            ServerSession::create(*this, _acceptor.get_executor().context());

    _acceptor.async_accept(session->getSocket(),
                           boost::bind(&Server::handleAccept, this, session,
                                       boost::asio::placeholders::error));
}

void Server::handleAccept(ServerSession::SessionPointer session,
                          const boost::system::error_code &error)
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

//
// Created by entropy on 9/18/18.
//

#ifndef CPP_BABEL_2018_SERVER_H
#define CPP_BABEL_2018_SERVER_H

#include <set>
#include <thread>
#include <boost/asio/ip/tcp.hpp>

#include "Logger.h"
#include "CommandLexer.h"
#include "ServerException.h"

using boost::asio::ip::tcp;

class ServerSession;
typedef boost::shared_ptr<ServerSession> ServerSessionPointer;
class User;
typedef boost::shared_ptr<User> UserPointer;
class Message;
class CommandLexer;

class Server {
public:
	Server(unsigned short port);
	~Server();

	void start();
	void stop();
	void broadcast(const BabelString &message);
	void broadcast(const BabelString &message, ServerSession *session);
	User *getUser(const BabelString &name);
	User *newUser(const BabelString &name);
	CommandLexer &getLexer();
	std::vector<User *> &getOnlineUsers();

private:
	void startAccept();
	void handleAccept(ServerSessionPointer, const boost::system::error_code &error);

	std::set<ServerSessionPointer> _sessions;
	std::set<UserPointer> _users;
	boost::asio::io_context _io_context;
	tcp::endpoint _endpoint;
	tcp::acceptor _acceptor;
	std::thread _mainThread;
	CommandLexer _lexer;
	std::vector<User *> _onlineUsers;
	bool _active;
};

#include "ServerSession.h"
#include "User.h"
#include "Message.h"

#endif //CPP_BABEL_2018_SERVER_H

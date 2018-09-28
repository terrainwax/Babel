//
// Created by entropy on 9/18/18.
//

#ifndef CPP_BABEL_2018_USER_H
#define CPP_BABEL_2018_USER_H

#include <set>
#include <iostream>
#include <deque>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "BabelString.h"

#ifdef _WIN32
	#include "Message.h"
#endif

class Server;

class ServerSession;
typedef boost::shared_ptr<ServerSession> ServerSessionPointer;

#ifndef _WIN32
	class Message;
#endif
typedef std::deque<Message> MessageQueue;

class User : public boost::enable_shared_from_this<User> {

public:
	typedef boost::shared_ptr<User> UserPointer;

	static UserPointer create(Server &_server, const BabelString &name);

	void setName(const BabelString &name);
	void addSession(ServerSessionPointer session);
	void removeSession(ServerSessionPointer session);
	void transmit(const Message &message);
	BabelString getName();
	BabelString getPassword();
	void setPassword(const BabelString &password);
	bool getStatus() const;
	void setStatus(bool status);
	unsigned char getID() const;
	void setID(unsigned char id);

private:
	explicit User(Server &_server, const BabelString &name);

	std::set<ServerSessionPointer> _sessions;
	enum {
		max_recent_messages = 100
	};
	MessageQueue _recentMessageQueue;

	Server &_server;
	BabelString _name;
	BabelString _password;
	bool _isAvailable;
	unsigned char _id;
};

#include "Server.h"
#include "ServerSession.h"
#include "Message.h"

#endif //CPP_BABEL_2018_USER_H

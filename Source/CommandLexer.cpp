/*
** EPITECH PROJECT, 2018
** CPP_babel_2018
** File description:
** CommandLexer.cpp
*/

#include <iostream>
#include "CommandLexer.h"

CommandLexer::CommandLexer(Server &server) : _server(server)
{
	_functionMap[CommandIdentifier::LOGIN] = std::bind(&CommandLexer::login, this, std::placeholders::_1, std::placeholders::_2);
	_functionMap[CommandIdentifier::ONLINE] = std::bind(&CommandLexer::online, this, std::placeholders::_1, std::placeholders::_2);
	_functionMap[CommandIdentifier::OFFLINE] = std::bind(&CommandLexer::offline, this, std::placeholders::_1, std::placeholders::_2);
	_functionMap[CommandIdentifier::HOST] = std::bind(&CommandLexer::host, this, std::placeholders::_1, std::placeholders::_2);
	_functionMap[CommandIdentifier::CALL] = std::bind(&CommandLexer::call, this, std::placeholders::_1, std::placeholders::_2);
	_functionMap[CommandIdentifier::HANG] = std::bind(&CommandLexer::hang, this, std::placeholders::_1, std::placeholders::_2);
	_functionMap[CommandIdentifier::ALIVE] = std::bind(&CommandLexer::alive, this, std::placeholders::_1, std::placeholders::_2);
	_functionMap[CommandIdentifier::LIST] = std::bind(&CommandLexer::list, this, std::placeholders::_1, std::placeholders::_2);
}

void CommandLexer::parse(Packet &packet, ServerSession *session)
{
	auto command = (Command *) packet.body();
	auto function = _functionMap[command->data.id];
	if (function)
		function(packet, session);
	else
		ko();
}

void CommandLexer::sendAnswer(std::string answer, ServerSession *session)
{
	Packet packet;

	packet.bodyLength(answer.size());
	std::strcpy(packet.body(), answer.c_str());
	packet.encodeHeader();
	session->deliver(packet);
}

void CommandLexer::online(Packet &packet, ServerSession *session)
{
	if (!session->hasUser())
	ko();

	std::vector<User *> _sameUsernameUsers;

	for (auto user : _server.getOnlineUsers())
	{
		if (user == session->getUser())
		ko();
		if (user->getName() == session->getUser()->getName())
			_sameUsernameUsers.emplace_back(user);
	}

	auto isTaken = [&](unsigned char id) -> bool {
		for (auto user : _sameUsernameUsers)
			if (user->getID() == id)
				return true;
		return false;
	};

	unsigned char id = 1;
	for (; id < UCHAR_MAX && isTaken(id); id++);
	if (id == UCHAR_MAX)
		ko();

	session->getUser()->setID(id);
	_server.getOnlineUsers().emplace_back(session->getUser());
	char string_id[4] = {0};
	std::snprintf(string_id, 4, "%03u", id);
	sendAnswer(std::string("OK ") + string_id, session);
}

void CommandLexer::offline(Packet &packet, ServerSession *session)
{
	if (!session->hasUser())
	ko();

	for (auto user = _server.getOnlineUsers().begin(); user != _server.getOnlineUsers().end(); user++)
	{
		if (*user == session->getUser())
		{
			user = _server.getOnlineUsers().erase(user);
			sendAnswer("OK", session);
			return;
		}
	}

	ko();
}

void CommandLexer::host(Packet &packet, ServerSession *session)
{
	sendAnswer("OK", session);
	session->getUser()->setStatus(false);
}

void CommandLexer::call(Packet &packet, ServerSession *session)
{
	sendAnswer("OK", session);
	session->getUser()->setStatus(false);
}

void CommandLexer::hang(Packet &packet, ServerSession *session)
{
	sendAnswer("OK", session);
	session->getUser()->setStatus(true);
}

void CommandLexer::alive(Packet &packet, ServerSession *session)
{
	sendAnswer("OK", session);
}

void CommandLexer::list(Packet &packet, ServerSession *session)
{
	std::string answer("OK");

	for (auto user : _server.getOnlineUsers())
	{
		if (user == session->getUser())
			continue;
		char line[1024];
		std::snprintf(line, 1024, "\n%s#%03u %s", user->getName().c_str(),
			user->getID(), user->getStatus() ? "available" : "busy");
		answer.append(line);
	}

	if (answer.size() != std::string("OK").size())
		answer.append("\n");
	sendAnswer(answer, session);
}

void CommandLexer::login(Packet &packet, ServerSession *session)
{
	auto logged_in = [&] (std::string user) {
		std::cout << "User " << user << " logged in." << std::endl;
		sendAnswer("OK", session);
	};
	std::string args(((Command *)packet.body())->data.data + sizeof(CommandIdentifier));
	auto tokens = tokenize(args);
	auto username = *tokens.begin();

	// yeah there isn't any other way, cpp is amazing -victor
	auto passwordPtr = tokens.begin();
	std::string password;
	passwordPtr++;
	if (passwordPtr != tokens.end())
		password = *passwordPtr;

	if (session->hasUser()) {
		session->getUser()->setName(username);
		logged_in(username);
		return;
	}

	User *user = _server.getUser(username);
	if (_server.getUser(username) == nullptr) {
		user = _server.newUser(username);
		session->setUser(user);
		if (!password.empty())
			user->setPassword(password);
		logged_in(username);
	} else {
		if (user->getPassword() == password) {
			session->setUser(user);
			logged_in(username);
		} else {
			std::cout << "ServerSession " << session
				<< " failed login with username "
				<< username << " and password "
				<< password << std::endl;
			ko();
		}
	}
}

CommandLexer::Tokens CommandLexer::tokenize(std::string &toTokenize)
{
	boost::char_separator<char> separator(" \t");
	return Tokens(toTokenize, separator);
}

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

void CommandLexer::parse(BabelString &message, ServerSession *session)
{
	auto command = (Command *) message.getData();
	auto function = _functionMap[command->data.id];
	if (function)
		function(message, session);
	else
		ko();
}

void CommandLexer::sendAnswer(BabelString answer, ServerSession *session)
{
	session->deliver(answer);
}

void CommandLexer::online(BabelString &message, ServerSession *session)
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
	sendAnswer(BabelString("OK ") + string_id, session);
}

void CommandLexer::offline(BabelString &message, ServerSession *session)
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

void CommandLexer::host(BabelString &message, ServerSession *session)
{
	sendAnswer("OK", session);
	session->getUser()->setStatus(false);
}

void CommandLexer::call(BabelString &message, ServerSession *session)
{
	sendAnswer("OK", session);
	session->getUser()->setStatus(false);
}

void CommandLexer::hang(BabelString &message, ServerSession *session)
{
	sendAnswer("OK", session);
	session->getUser()->setStatus(true);
}

void CommandLexer::alive(BabelString &message, ServerSession *session)
{
	sendAnswer("OK", session);
}

void CommandLexer::list(BabelString &message, ServerSession *session)
{
	BabelString answer("OK");

	for (auto user : _server.getOnlineUsers())
	{
		if (user == session->getUser())
			continue;
		char line[1024];
		std::snprintf(line, 1024, "\n%s#%03u %s", user->getName().getData(),
			user->getID(), user->getStatus() ? "available" : "busy");
		answer.append(line);
	}

	if (answer.getSize() != BabelString("OK").getSize())
		answer.append("\n");

	sendAnswer(answer, session);
}

void CommandLexer::login(BabelString &message, ServerSession *session)
{
	auto logged_in = [&] (User *user) {
		std::cout << "User " << user->getName() << " logged in." << std::endl;
		sendAnswer("OK", session);
	};

	BabelString args(((Command *)message.getData())->data.data + sizeof(CommandIdentifier));
	auto tokens = tokenize(args);
	auto username = BabelString((*tokens.begin()).c_str());

	// yeah there isn't any other way, cpp is amazing -victor
	auto passwordPtr = tokens.begin();
	BabelString password;
	passwordPtr++;
	if (passwordPtr != tokens.end())
		password = BabelString((*passwordPtr).c_str());

	if (session->hasUser()) {
		session->getUser()->setName(username);
		logged_in(session->getUser());
		return;
	}

	User *user = _server.getUser(username);
	if (_server.getUser(username) == nullptr) {
		user = _server.newUser(username);
		session->setUser(user);
		if (!password.empty())
			user->setPassword(password);
		logged_in(session->getUser());
	} else {
		if (user->getPassword() == password) {
			session->setUser(user);
			logged_in(session->getUser());
		} else {
			std::cout << "ServerSession " << session
				<< " failed login with username "
				<< username << " and password "
				<< password << std::endl;
			ko();
		}
	}
}

CommandLexer::Tokens CommandLexer::tokenize(BabelString &toTokenize)
{
	boost::char_separator<char> separator(" \t");
	return Tokens(std::string(toTokenize.getData()), separator);
}

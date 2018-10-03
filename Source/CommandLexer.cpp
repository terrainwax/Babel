/*
** EPITECH PROJECT, 2018
** CPP_babel_2018
** File description:
** CommandLexer.cpp
*/

#include <boost/lexical_cast.hpp>
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

	if (command->magic != COMMAND_MAGIC)
	ko();

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

	for (const auto &user : _server.getOnlineUsers())
		if (session->getUser() == user)
		{
			sendAnswer("OK", session);
			return;
		}
	_server.getOnlineUsers().emplace_back(session->getUser());
	sendAnswer("OK", session);
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
	if (!session->hasUser())
	ko();

	if (session->getUser()->getStatus() != User::Status::BUSY)
	ko();

	sendAnswer("OK", session);
	session->getUser()->setStatus(User::Status::HOSTING);
}

void CommandLexer::call(BabelString &message, ServerSession *session)
{
	if (!session->hasUser())
	ko();

	if (session->getUser()->getStatus() != User::Status::AVAILABLE)
	ko();

	// fuck my life
	auto command = (Command *)message.getData();
	std::string args(command->data.data + sizeof(CommandIdentifier));
	auto tokens = tokenize(args);
	auto usernamePtr = tokens.begin();
	if (usernamePtr == tokens.end())
	ko();
	auto username = BabelString(usernamePtr->c_str());
	usernamePtr++;
	if (usernamePtr == tokens.end())
	ko();
	auto portString = BabelString(usernamePtr->c_str());
	auto user = _server.getUser(username);
	if (user->getStatus() != User::Status::AVAILABLE)
	ko();

	auto answerMessage = Message(BabelString("CALL ")
		+ session->getUser()->getName() + " " +
		session->getAddress() + " " + portString, session);
	user->transmit(answerMessage);
	sendAnswer("OK", session);
	session->getUser()->setStatus(User::Status::BUSY);
	user->setStatus(User::Status::BUSY);
}

void CommandLexer::hang(BabelString &message, ServerSession *session)
{
	if (!session->hasUser())
	ko();

	if (session->getUser()->getStatus() == User::Status::AVAILABLE)
	ko();

	sendAnswer("OK", session);
	session->getUser()->setStatus(User::Status::AVAILABLE);
}

void CommandLexer::alive(BabelString &message, ServerSession *session)
{
	sendAnswer("OK", session);
}

void CommandLexer::list(BabelString &message, ServerSession *session)
{
	if (!session->hasUser())
	ko();

	BabelString answer("OK");

	for (auto user : _server.getOnlineUsers())
	{
		if (user == session->getUser())
			continue;
		char line[1024];
		std::snprintf(line, 1024, "\n%s %s", user->getName().getData(),
			user->getStatus() == User::Status::AVAILABLE ? "Available"
				: user->getStatus() == User::Status::BUSY ? "Busy"
				: "Hosting");
		answer.append(line);
	}

	sendAnswer(answer, session);
}

void CommandLexer::login(BabelString &message, ServerSession *session)
{
	auto logged_in = [&] (User *user) {
		Logger::get()->debug(BabelString("User ") + user->getName() + BabelString(" Is Connected"));
		sendAnswer("OK", session);
	};

	auto command = (Command *)message.getData();
	std::string args(command->data.data + sizeof(CommandIdentifier));
	auto tokens = tokenize(args);
	auto username = BabelString(tokens.begin()->c_str());

	// yeah there isn't any other way, cpp is amazing -victor
	auto passwordPtr = tokens.begin();
	BabelString password;
	passwordPtr++;
	if (passwordPtr != tokens.end())
		password = BabelString(passwordPtr->c_str());

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

CommandLexer::Tokens CommandLexer::tokenize(std::string &toTokenize)
{
	boost::char_separator<char> separator(" \t");
	return Tokens(toTokenize, separator);
}

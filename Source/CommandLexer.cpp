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
	_functionMap[CommandIdentifier::JOIN] = std::bind(&CommandLexer::join, this, std::placeholders::_1, std::placeholders::_2);
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
		if (session->getUser() == user) {
			sendAnswer("OK", session);
			return;
		}
	_server.getOnlineUsers().emplace_back(session->getUser());
	sendAnswer("OK", session);
	Logger::get()->debug(BabelString("User ") + session->getUser()->getName() + " now online");
	broadcast(BabelString("LIST ") + session->getUser()->getName() + " Available", session);
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
			Logger::get()->debug(BabelString("User ") + session->getUser()->getName() + " now offline");
			broadcast(BabelString("LIST ") + session->getUser()->getName() + " Offline", session);
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
	Logger::get()->debug(session->getUser()->getName() + " now hosting a call");
	broadcast(BabelString("LIST ") + session->getUser()->getName() + " Hosting", session);
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

	session->getUser()->hostCall(session->getAddress(), portString);

	auto answerMessage = Message(BabelString("CALL ")
		+ session->getUser()->getName(), session);
	user->transmit(answerMessage);
	sendAnswer(BabelString("OK ") + user->getRecentSession()->getAddress(), session);
	session->getUser()->setStatus(User::Status::CALLING);
	Logger::get()->debug(session->getUser()->getName() + BabelString(" calling ") + user->getName());
}

void CommandLexer::hang(BabelString &message, ServerSession *session)
{
	if (!session->hasUser())
	ko();

	auto command = (Command *)message.getData();
	auto username = BabelString(command->data.data + sizeof(CommandIdentifier));
	if (!username.empty()) {
		auto user = _server.getUser(username);
		if (user == nullptr || user->getStatus() != User::Status::CALLING)
			ko();
		auto message = Message(BabelString("HANG"), session);
		user->setStatus(User::Status::AVAILABLE);
		user->transmit(message);
		sendAnswer("OK", session);
		return;
	}

	if (session->getUser()->getStatus() == User::Status::AVAILABLE)
	ko();

	if (session->getUser()->getCall() != nullptr)
		session->getUser()->stopCall();

	sendAnswer("OK", session);
	session->getUser()->setStatus(User::Status::AVAILABLE);
	Logger::get()->debug(session->getUser()->getName() + "hung up the call");
	broadcast(BabelString("LIST ") + session->getUser()->getName() + " Available", session);
}

void CommandLexer::alive(BabelString &message, ServerSession *session)
{
	sendAnswer("OK", session);
}

void CommandLexer::list(BabelString &message, ServerSession *session)
{
	if (!session->hasUser())
	ko();

	sendAnswer("OK", session);

	for (auto user : _server.getOnlineUsers())
	{
		if (user == session->getUser())
			continue;
		char line[1024];
		std::snprintf(line, 1024, "LIST %s %s", user->getName().getData(),
			user->getStatus() == User::Status::AVAILABLE ? "Available"
				: user->getStatus() == User::Status::BUSY
				|| user->getStatus() == User::Status::CALLING ? "Busy"
				: "Hosting");
		sendAnswer(BabelString(line), session);
	}

	Logger::get()->debug(session->getUser()->getName() + " listed online users.");
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

void CommandLexer::join(BabelString &message, ServerSession *session)
{
	auto command = (Command *)message.getData();
	auto args = std::string(command->data.data + sizeof(CommandIdentifier));
	auto tokens = tokenize(args);
	auto username = BabelString(tokens.begin()->c_str());

	auto portPtr = tokens.begin();
	BabelString port;
	portPtr++;
	if (portPtr != tokens.end())
		port = BabelString(portPtr->c_str());

	if (username.empty())
	ko();

	auto userToJoin = _server.getUser(username);
	if (userToJoin == nullptr || userToJoin->getStatus() != User::Status::CALLING)
	ko();

	auto answer = BabelString("OK ") + userToJoin->getCall()->getIP() +
		BabelString(" ") + userToJoin->getCall()->getPort();
	sendAnswer(answer, session);
	userToJoin->setStatus(User::Status::BUSY);
	session->getUser()->setStatus(User::Status::BUSY);

	auto answerUserToJoin = BabelString("JOIN ") + session->getUser()->getName() + BabelString(" ") + port;
	userToJoin->transmit(Message(answerUserToJoin, session));

	broadcast(BabelString("LIST ") + session->getUser()->getName() + " Busy", session);
	broadcast(BabelString("LIST ") + userToJoin->getName() + " Busy", session);
}

CommandLexer::Tokens CommandLexer::tokenize(std::string &toTokenize)
{
	boost::char_separator<char> separator(" \t");
	return Tokens(toTokenize, separator);
}

void CommandLexer::broadcast(BabelString answer, ServerSession *session)
{
	_server.broadcast(answer, session);
}

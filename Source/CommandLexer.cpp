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
	_functionMap["ONLINE"] = std::bind(&CommandLexer::online, this, std::placeholders::_1, std::placeholders::_2);
	_functionMap["OFFLINE"] = std::bind(&CommandLexer::offline, this, std::placeholders::_1, std::placeholders::_2);
	_functionMap["HOST"] = std::bind(&CommandLexer::host, this, std::placeholders::_1, std::placeholders::_2);
	_functionMap["CALL"] = std::bind(&CommandLexer::call, this, std::placeholders::_1, std::placeholders::_2);
	_functionMap["HANG"] = std::bind(&CommandLexer::hang, this, std::placeholders::_1, std::placeholders::_2);
	_functionMap["ALIVE"] = std::bind(&CommandLexer::alive, this, std::placeholders::_1, std::placeholders::_2);
	_functionMap["LIST"] = std::bind(&CommandLexer::list, this, std::placeholders::_1, std::placeholders::_2);
}

void CommandLexer::parse(Packet &packet, ServerSession *session)
{
	auto rawCommand = std::string(packet.str());
	if (rawCommand.empty())
		ko();

	auto tokens = tokenize(rawCommand);
	auto command = *tokens.begin();

	if (command.empty())
		ko();

	auto function = _functionMap[command];
	if (function)
		function(tokens, session);
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

boost::tokenizer<boost::char_separator<char>> CommandLexer::tokenize(std::string &toTokenize)
{
	boost::char_separator<char> separator(" \t");
	return Tokens(toTokenize, separator);
}

void CommandLexer::online(CommandLexer::Tokens tokens, ServerSession *session)
{
	if (!session->hasUser())
		ko();

	for (auto user : _server.getOnlineUsers())
	{
		if (user == session->getUser())
			ko();
	}

	_server.getOnlineUsers().emplace_back(session->getUser());
	sendAnswer("OK", session);
	(void)tokens;
}

void CommandLexer::offline(CommandLexer::Tokens tokens, ServerSession *session)
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
	(void)tokens;
}

void CommandLexer::host(CommandLexer::Tokens tokens, ServerSession *session)
{
	sendAnswer("OK", session);
	session->getUser()->setStatus(false);
}

void CommandLexer::call(CommandLexer::Tokens tokens, ServerSession *session)
{
	sendAnswer("OK", session);
	session->getUser()->setStatus(false);
}

void CommandLexer::hang(CommandLexer::Tokens tokens, ServerSession *session)
{
	sendAnswer("OK", session);
	session->getUser()->setStatus(true);
}

void CommandLexer::alive(CommandLexer::Tokens tokens, ServerSession *session)
{
	sendAnswer("OK", session);
}

void CommandLexer::list(CommandLexer::Tokens tokens, ServerSession *session)
{
	std::string answer("OK");

	for (auto user : _server.getOnlineUsers())
	{
		if (user == session->getUser())
			continue;
		answer.append("\n");
		answer.append(user->getName());
		answer.append(" ");
		answer.append(user->getStatus() ? "available" : "busy");
	}

	if (answer.size() != std::string("OK").size())
		answer.append("\n");
	sendAnswer(answer, session);
}

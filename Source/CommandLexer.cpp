/*
** EPITECH PROJECT, 2018
** CPP_babel_2018
** File description:
** CommandLexer.cpp
*/

#include <iostream>
#include "CommandLexer.h"

CommandLexer::CommandLexer()
{
	_functionMap["ONLINE"] = std::bind(&CommandLexer::sendAnswer, this, "OK", std::placeholders::_1);
	_functionMap["OFFLINE"] = std::bind(&CommandLexer::sendAnswer, this, "OK", std::placeholders::_1);
	_functionMap["HOST"] = std::bind(&CommandLexer::sendAnswer, this, "OK", std::placeholders::_1);
	_functionMap["CALL"] = std::bind(&CommandLexer::sendAnswer, this, "OK", std::placeholders::_1);
	_functionMap["HANG"] = std::bind(&CommandLexer::sendAnswer, this, "OK", std::placeholders::_1);
	_functionMap["ALIVE"] = std::bind(&CommandLexer::sendAnswer, this, "OK", std::placeholders::_1);
	_functionMap["LIST"] = std::bind(&CommandLexer::sendAnswer, this, "OK", std::placeholders::_1);
}

void CommandLexer::parse(Packet &packet, ServerSession *session)
{
	auto rawCommand = std::string(packet.str());
	if (rawCommand.empty())
	{
		sendAnswer("KO", session);
		return;
	}

	auto tokens = tokenize(rawCommand);
	auto command = *tokens.begin();

	if (command.empty())
	{
		sendAnswer("KO", session);
		return;
	}

	auto function = _functionMap[command];
	if (function)
		function(session);
	else
	{
		sendAnswer("KO", session);
		return;
	}
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
	return boost::tokenizer<boost::char_separator<char>>(toTokenize, separator);
}
/*
** EPITECH PROJECT, 2018
** CPP_babel_2018
** File description:
** CommandLexer.h
*/

#ifndef CPP_BABEL_2018_COMMANDLEXER_H
#define CPP_BABEL_2018_COMMANDLEXER_H

#include <map>
#include <functional>
#include <boost/tokenizer.hpp>
#include "Packet.h"

class ServerSession;

class CommandLexer {
public:
	CommandLexer();
	~CommandLexer() = default;

	void parse(Packet &packet, ServerSession *session);
private:
	void sendAnswer(std::string answer, ServerSession *session);
	boost::tokenizer<boost::char_separator<char>> tokenize(std::string &toTokenize);
	void OK(ServerSession *session);

	std::map<std::string, std::function<void(ServerSession *)>> _functionMap;
};

#include "ServerSession.h"

#endif //CPP_BABEL_2018_COMMANDLEXER_H

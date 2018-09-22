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

#define ko()	{ sendAnswer("KO", session); return; }

class ServerSession;
class Server;

class CommandLexer {
public:
	CommandLexer(Server &server);
	~CommandLexer() = default;

	void parse(Packet &packet, ServerSession *session);
private:
	typedef boost::tokenizer<boost::char_separator<char>> Tokens;

	void sendAnswer(std::string answer, ServerSession *session);
	Tokens tokenize(std::string &toTokenize);

	void online(Tokens tokens, ServerSession *session);
	void offline(Tokens tokens, ServerSession *session);
	void host(Tokens tokens, ServerSession *session);
	void call(Tokens tokens, ServerSession *session);
	void hang(Tokens tokens, ServerSession *session);
	void alive(Tokens tokens, ServerSession *session);
	void list(Tokens tokens, ServerSession *session);

	Server &_server;
	std::map<std::string, std::function<void(Tokens, ServerSession *)>> _functionMap;
};

#include "Server.h"

#endif //CPP_BABEL_2018_COMMANDLEXER_H

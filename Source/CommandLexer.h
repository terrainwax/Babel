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
#include "BabelString.h"
#include "Command.h"

#define ko()	{ sendAnswer("KO", session); return; }

class ServerSession;
class Server;

class CommandLexer {
public:
	explicit CommandLexer(Server &server);
	~CommandLexer() = default;

	void parse(BabelString &message, ServerSession *session);
private:
	typedef boost::tokenizer<boost::char_separator<char>> Tokens;

	void sendAnswer(BabelString answer, ServerSession *session);
	Tokens tokenize(std::string &toTokenize);

	void login(BabelString &message, ServerSession *session);
	void online(BabelString &message, ServerSession *session);
	void offline(BabelString &message, ServerSession *session);
	void host(BabelString &message, ServerSession *session);
	void call(BabelString &message, ServerSession *session);
	void hang(BabelString &message, ServerSession *session);
	void alive(BabelString &message, ServerSession *session);
	void list(BabelString &message, ServerSession *session);
	void join(BabelString &message, ServerSession *session);

	Server &_server;
	std::map<CommandIdentifier, std::function<void(BabelString &, ServerSession *)>> _functionMap;
};

#include "Server.h"

#endif //CPP_BABEL_2018_COMMANDLEXER_H

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
#include "Command.h"

#define ko()	{ sendAnswer("KO", session); return; }

class ServerSession;
class Server;

class CommandLexer {
public:
	explicit CommandLexer(Server &server);
	~CommandLexer() = default;

	void parse(Packet &packet, ServerSession *session);
private:
	typedef boost::tokenizer<boost::char_separator<char>> Tokens;

	void sendAnswer(BabelString answer, ServerSession *session);
	Tokens tokenize(BabelString &toTokenize);

	void login(Packet &packet, ServerSession *session);
	void online(Packet &packet, ServerSession *session);
	void offline(Packet &packet, ServerSession *session);
	void host(Packet &packet, ServerSession *session);
	void call(Packet &packet, ServerSession *session);
	void hang(Packet &packet, ServerSession *session);
	void alive(Packet &packet, ServerSession *session);
	void list(Packet &packet, ServerSession *session);

	Server &_server;
	std::map<CommandIdentifier, std::function<void(Packet &, ServerSession *)>> _functionMap;
};

#include "Server.h"

#endif //CPP_BABEL_2018_COMMANDLEXER_H

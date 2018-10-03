/*
** EPITECH PROJECT, 2018
** CPP_babel_2018
** File description:
** Call.hpp
*/

#ifndef CPP_BABEL_2018_CALL_HPP
#define CPP_BABEL_2018_CALL_HPP

#include "User.h"

class Call {
public:
	Call(User *host, BabelString ip, BabelString port);
	~Call() = default;
	
	const BabelString &getPort();
	const BabelString &getIP();
private:
	User *_host;
	BabelString _ip;
	BabelString _port;
};

#endif //CPP_BABEL_2018_CALL_HPP

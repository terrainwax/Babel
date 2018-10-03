/*
** EPITECH PROJECT, 2018
** CPP_babel_2018
** File description:
** Call.cpp
*/

#include "Call.h"

Call::Call(User *host, BabelString ip, BabelString port) : _host(host), _ip(ip), _port(port)
{
}

const BabelString &Call::getIP()
{
	return _ip;
}

const BabelString &Call::getPort()
{
	return _port;
}

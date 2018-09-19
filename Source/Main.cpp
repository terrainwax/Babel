/*
** EPITECH PROJECT, 2018
** CPP_babel_2018
** File description:
** Main.cpp
*/

#include "Server.hpp"

int main(int argc, char *argv[])
{
	Server server(42069);

	server.run();
	return 0;
}
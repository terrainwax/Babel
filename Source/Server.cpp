/*
** EPITECH PROJECT, 2018
** CPP_babel_2018
** File description:
** Server.cpp
*/

#include "Server.hpp"

Server::Server(unsigned short port) : _io_context(), _acceptor(_io_context, tcp::endpoint(tcp::v4(), port))
{
	startAccept();
}

Server::~Server() = default;

void Server::run()
{
	_io_context.run();
}

void Server::startAccept()
{
	Connection::pointer new_connection =
		Connection::create(_acceptor.get_executor().context());

	_acceptor.async_accept(new_connection->socket(),
		boost::bind(&Server::handleAccept, this, new_connection,
			boost::asio::placeholders::error));
}

void Server::handleAccept(Connection::pointer new_connection,
	const boost::system::error_code &error
)
{
	if (!error)
		new_connection->start();

	startAccept();
}

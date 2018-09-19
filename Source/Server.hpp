/*
** EPITECH PROJECT, 2018
** CPP_babel_2018
** File description:
** Server.hpp
*/

#ifndef CPP_BABEL_2018_SERVER_HPP
#define CPP_BABEL_2018_SERVER_HPP

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include "Connection.hpp"

using boost::asio::ip::tcp;

class Server {
public:
	explicit Server(unsigned short port);
	~Server();
	void run();
private:
	void startAccept();
	void handleAccept(Connection::pointer new_connection,
		const boost::system::error_code &error
	);

	boost::asio::io_context _io_context;
	tcp::acceptor _acceptor;
};

#endif //CPP_BABEL_2018_SERVER_HPP

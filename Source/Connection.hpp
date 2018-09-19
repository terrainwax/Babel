/*
** EPITECH PROJECT, 2018
** CPP_babel_2018
** File description:
** Connection.hpp
*/

#ifndef CPP_BABEL_2018_CONNECTION_HPP
#define CPP_BABEL_2018_CONNECTION_HPP

#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/bind.hpp>

using boost::asio::ip::tcp;

class Connection : public boost::enable_shared_from_this<Connection>
{
public:
	typedef boost::shared_ptr<Connection> pointer;

	static pointer create(boost::asio::io_context& io_context);
	tcp::socket& socket();
	void start();

private:
	explicit Connection(boost::asio::io_context& io_context);
	void handle_write(const boost::system::error_code& error, size_t bytes);

	tcp::socket _socket;
};

#endif //CPP_BABEL_2018_CONNECTION_HPP

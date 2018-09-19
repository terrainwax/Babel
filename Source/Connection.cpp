/*
** EPITECH PROJECT, 2018
** CPP_babel_2018
** File description:
** Connection.cpp
*/

#include <iostream>
#include "Connection.hpp"

Connection::pointer Connection::create(boost::asio::io_context &io_context)
{
	return pointer(new Connection(io_context));
}

tcp::socket &Connection::socket()
{
	return _socket;
}

void Connection::start()
{
	auto _message = std::string("Heya");

	boost::asio::async_write(_socket, boost::asio::buffer(_message),
		boost::bind(&Connection::handle_write, shared_from_this(),
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
}

Connection::Connection(boost::asio::io_context &io_context) : _socket(io_context)
{

}

void Connection::handle_write(const boost::system::error_code &error, size_t bytes)
{
	std::cout << "Written to someone" << std::endl;
}

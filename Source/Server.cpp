//
// Created by entropy on 9/18/18.
//

#include "Server.h"

Server::Server(unsigned short port)
    : _io_context(), _endpoint(tcp::endpoint(tcp::v4(), port)), _acceptor(_io_context, _endpoint), _user()
{
    startAccept();
}

void Server::run()
{
    _io_context.run();
}

void Server::startAccept()
{
    Session::SessionPointer session =
            Session::create(_acceptor.get_executor().context(), _user);

    _acceptor.async_accept(session->getSocket(),
                           boost::bind(&Server::handleAccept, this, session,
                                       boost::asio::placeholders::error));
}

void Server::handleAccept(Session::SessionPointer session,
                          const boost::system::error_code &error)
{
    if (!error)
        session->start();

    startAccept();
}
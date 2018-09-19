//
// Created by entropy on 9/18/18.
//

#include "Server.h"

Server::Server(boost::asio::io_service &io_service,
                       const tcp::endpoint &endpoint)
        : _acceptor(io_service, endpoint),
          _socket(io_service) {
    do_accept();
}

void Server::do_accept() {
    _acceptor.async_accept(_socket,
                           [this](boost::system::error_code ec) {
                               if (!ec) {
                                   std::make_shared<Session>(std::move(_socket), _room)->start();
                               }

                               do_accept();
                           });
}
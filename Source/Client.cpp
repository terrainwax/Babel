//
// Created by entropy on 9/18/18.
//

#include "Client.h"

Client::Client(const std::string &username,
                    const std::string &password,
                       const std::string &address,
                       unsigned short port)
        : _username(username),
        _password(password),
        _io_context(),
          _endpoint(boost::asio::ip::address::from_string(address), port)
          {

    startConnect();
}

void Client::run()
{
    _mainThread = std::thread([this]() { _io_context.run(); });
}

void Client::startConnect() {

    ClientSession::SessionPointer session = ClientSession::create(*this, _io_context);

    boost::asio::async_connect(session->getSocket(), &_endpoint,
                               boost::bind(&Client::handleConnect, this, session,
                                           boost::asio::placeholders::error));
}

void Client::handleConnect(ClientSession::SessionPointer session,
                          const boost::system::error_code &error)
{
    if (!error) {
        std::cout << "ClientSession created with: " << session->getAddress() << std::endl;
        _sessions.insert(session);
        session->start();
    }
}

void Client::display(Message message)
{
    std::cout << "Received message: " << message << std::endl;
}

void Client::write(const Packet &msg) {
    for (auto session: _sessions)
        session->deliver(msg);
}

void Client::close() {
    //_io_context.post([this]() { _socket.close(); });
    _mainThread.join();
}



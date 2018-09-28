/*
** EPITECH PROJECT, 2018
** CPP_babel_2018
** File description:
** ClientSession.cpp
*/

#include <iostream>
#include "ClientSession.h"


ClientSession::ClientSession(Client &client, boost::asio::io_context &io_context) : _client(client), _socket(io_context), _crypto()
{
}


ClientSession::SessionPointer ClientSession::create(Client &client, boost::asio::io_context &io_context)
{
    return SessionPointer(new ClientSession(client, io_context));
}

tcp::socket &ClientSession::getSocket()
{
    return _socket;
}

std::string ClientSession::getAddress() const
{
    return _socket.remote_endpoint().address().to_string();
}

void ClientSession::open() {
    std::cout << "ClientSession opened: " << getAddress() << std::endl;
    startReadHeader();
}

void ClientSession::startReadHeader()
{
    boost::asio::async_read(_socket, boost::asio::buffer(_readMsg.data(), Packet::header_length),
                            boost::bind(&ClientSession::handleReadHeader, shared_from_this(),
                                        boost::asio::placeholders::error,
                                        boost::asio::placeholders::bytes_transferred));
}

void ClientSession::handleReadHeader(const boost::system::error_code &error, size_t bytes)
{
    if (!error && _readMsg.decodeHeader()) {
        startReadBody();
    } else {
        //if (hasUser())
            //_user->removeSession(shared_from_this());
        //_socket.stop();
    }
}

void ClientSession::startReadBody()
{
    boost::asio::async_read(_socket, boost::asio::buffer(_readMsg.body(), _readMsg.bodyLength()),
                            boost::bind(&ClientSession::handleReadBody, shared_from_this(),
                                        boost::asio::placeholders::error,
                                        boost::asio::placeholders::bytes_transferred));
}

void ClientSession::handleReadBody(const boost::system::error_code &error, size_t bytes)
{
    if (!error) {
        if (_readMsg.str().substr(0, 26) == "-----BEGIN PUBLIC KEY-----")
        {
            std::cout << "Received RSA Public Key From Server." << std::endl;

            const std::string RSAPublicKey = _readMsg.str();
            _crypto.setRemotePublicKey(RSAPublicKey);

            /*

            std::string encryptionKey = std::string();
            std::string encryptionIv = std::string();

            std::string homo = _crypto.getAESKey();

            std::cout << "Hello: " << homo << std::endl;

            std::string encryptedAESKey = _crypto.encryptRSA(homo, encryptionKey, encryptionIv);

            deliver(encryptionKey);
            deliver(encryptionIv);
            deliver(encryptedAESKey);

            std::string encryptedAESIv = _crypto.encryptRSA(_crypto.getAESIv(), encryptionKey, encryptionIv);

            deliver(encryptionKey);
            deliver(encryptionIv);
            deliver(encryptedAESIv);

            */
        }
        _client.display(Message(_readMsg, nullptr));
        startReadHeader();
    } else {
        //if (hasUser())
            //_user->removeSession(shared_from_this());
    }
}

void ClientSession::startWrite()
{
    auto _message = std::string(_writeMessageQueue.front().data(),
                                _writeMessageQueue.front().length());

    boost::asio::async_write(_socket, boost::asio::buffer(_message),
                             boost::bind(&ClientSession::handleWrite, shared_from_this(),
                                         boost::asio::placeholders::error,
                                         boost::asio::placeholders::bytes_transferred));
}

void ClientSession::handleWrite(const boost::system::error_code &error, size_t bytes)
{
    if (!error) {
        _writeMessageQueue.pop_front();
        if (!_writeMessageQueue.empty()) {
            startWrite();
        }
    } else {
        //if (hasUser())
            //_user->removeSession(shared_from_this());
    }
}

void ClientSession::deliver(const std::string &message)
{
    Packet packet;
    packet.bodyLength(message.size());
    std::memcpy(packet.body(), message.data(), packet.bodyLength());
    packet.encodeHeader();

    bool write_in_progress = !_writeMessageQueue.empty();
    _writeMessageQueue.push_back(packet);

    if (!write_in_progress) {
        startWrite();
    }
}

std::ostream& operator<<(std::ostream& os, const ClientSession& session)
{
    os << session.getAddress();

    return os;
}
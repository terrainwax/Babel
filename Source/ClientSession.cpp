/*
** EPITECH PROJECT, 2018
** CPP_babel_2018
** File description:
** ClientSession.cpp
*/

#include <iostream>
#include "ClientSession.h"


ClientSession::ClientSession(Client &client, boost::asio::io_context &io_context) : _client(client), _socket(io_context), _crypto(), _secured(false)
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

BabelString ClientSession::getAddress() const
{
    return BabelString(_socket.remote_endpoint().address().to_string().c_str());
}

void ClientSession::open() {
    Logger::get()->debug(BabelString("ClientSession Opened: ") + getAddress());
    startReadHeader();
}

void ClientSession::close() {
    Logger::get()->debug(BabelString("ClientSession Closed: ") + getAddress());
    _socket.close();
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
        if (!_secured) {
            if (_readMsg.str().substr(0, 30) == "-----BEGIN RSA PUBLIC KEY-----") {
                const BabelString RSAPublicKey = _readMsg.str();
                _crypto.setRemotePublicKey(RSAPublicKey);

                std::cout << "Received RSA Public Key:" << std::endl << _crypto.getRemotePublicKey() << std::endl;

                BabelString aesKey = _crypto.getAESKey();

                std::cout << "Sending Encrypted AES Key: ";

                for (int i = 0; i < aesKey.getSize(); ++i)
                    std::cout << std::hex << std::setfill('0') << std::setw(2) << (unsigned int)(unsigned char)aesKey.getData()[i];

                std::cout << std::endl;

                BabelString encryptedAESKey = _crypto.encryptRSA(aesKey);

                deliver(BabelString("ENCRYPTED AES KEY:") + encryptedAESKey);

                BabelString aesIv = _crypto.getAESIv();

                std::cout << "Sending Encrypted AES Iv: ";

                for (int i = 0; i < aesIv.getSize(); ++i)
                    std::cout << std::hex << std::setfill('0') << std::setw(2) << (unsigned int)(unsigned char)aesIv.getData()[i];

                std::cout << std::endl;

                BabelString encryptedAESIv = _crypto.encryptRSA(aesIv);

                deliver(BabelString("ENCRYPTED AES IV:") + encryptedAESIv);

                Logger::get()->debug(BabelString("ClientSession Secured: ") + getAddress());

                _secured = true;
            }
        }
        else {
            BabelString decrypted = _crypto.decryptAES(_readMsg.str());
            _client.display(Message(decrypted, nullptr));
        }
        startReadHeader();
    } else {
        //if (hasUser())
            //_user->removeSession(shared_from_this());
    }
}

void ClientSession::startWrite()
{
    auto _message = BabelString(_writeMessageQueue.front().data(),
                                _writeMessageQueue.front().length());

    boost::asio::async_write(_socket, boost::asio::buffer(_message.getData(), _message.getSize()),
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

void ClientSession::deliver(const BabelString &message)
{
    BabelString data = message;

    if (_secured)
        data = _crypto.encryptAES(message);

    Packet packet;
    packet.bodyLength(data.getSize());
    std::memcpy(packet.body(), data.getData(), packet.bodyLength());
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
//
// Created by entropy on 9/18/18.
//

#include "User.h"

User::User(tcp::socket socket)
        : _socket(std::move(socket)) {
    _name = "Unknown";
    _address = _socket.remote_endpoint().address();
    _port = _socket.remote_endpoint().port();
}

void User::start(Room *room) {
    _room = room;
    _room->join(this);
    do_read_header();
}

void User::deliver(Message *message) {
    bool write_in_progress = !_writeMessageQueue.empty();
    _writeMessageQueue.push_back(message);
    if (!write_in_progress) {
        do_write();
    }
}

std::string User::getName() {
    return _name;
}

void User::do_read_header() {
    boost::asio::async_read(_socket,
                            boost::asio::buffer(_readPacket.data(), Packet::header_length),
                            [this](boost::system::error_code ec, std::size_t /*length*/) {
                                if (!ec && _readPacket.decodeHeader()) {
                                    do_read_body();
                                } else {
                                    std::cout << ec.message() << std::endl;
                                    _room->leave(this);
                                }
                            });
}

void User::do_read_body() {
    boost::asio::async_read(_socket,
                            boost::asio::buffer(_readPacket.body(), _readPacket.bodyLength()),
                            [this](boost::system::error_code ec, std::size_t /*length*/) {
                                if (!ec) {
                                    _room->shout(new Message(this, _readPacket));
                                    do_read_header();
                                } else {
                                    std::cout << ec.message() << std::endl;
                                    _room->leave(this);
                                }
                            });
}

void User::do_write() {
    boost::asio::async_write(_socket,
                             boost::asio::buffer(_writeMessageQueue.front()->getPacket().data(),
                                                 _writeMessageQueue.front()->getPacket().length()),
                             [this](boost::system::error_code ec, std::size_t /*length*/) {
                                 if (!ec) {
                                     _writeMessageQueue.pop_front();
                                     if (!_writeMessageQueue.empty()) {
                                         do_write();
                                     }
                                 } else {
                                     std::cout << ec.message() << std::endl;
                                     _room->leave(this);
                                 }
                             });
}
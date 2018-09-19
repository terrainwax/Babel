//
// Created by entropy on 9/18/18.
//

#include "Session.h"

Session::Session(tcp::socket socket, Room &room)
        : _socket(std::move(socket)),
          _room(room) {
    _name = _socket.remote_endpoint().address().to_string();
}

void Session::start() {
    _room.join(shared_from_this());
    do_read_header();
}

void Session::deliver(const Packet &msg) {
    bool write_in_progress = !_writeMsgQ.empty();
    _writeMsgQ.push_back(msg);
    if (!write_in_progress) {
        do_write();
    }
}

std::string Session::getName() {
    return _name;
}

void Session::do_read_header() {
    auto self(shared_from_this());
    boost::asio::async_read(_socket,
                            boost::asio::buffer(_readMsg.data(), Packet::header_length),
                            [this, self](boost::system::error_code ec, std::size_t /*length*/) {
                                if (!ec && _readMsg.decodeHeader()) {
                                    do_read_body();
                                } else {
                                    _room.leave(shared_from_this());
                                }
                            });
}

void Session::do_read_body() {
    auto self(shared_from_this());
    boost::asio::async_read(_socket,
                            boost::asio::buffer(_readMsg.body(), _readMsg.bodyLength()),
                            [this, self](boost::system::error_code ec, std::size_t /*length*/) {
                                if (!ec) {
                                    _room.transmit(shared_from_this(), _readMsg);
                                    if (_readMsg.str().substr(0, 5) == "NAME:")
                                        _name = _readMsg.str().substr(5, _readMsg.bodyLength() - 5);
                                    do_read_header();
                                } else {
                                    _room.leave(shared_from_this());
                                }
                            });
}

void Session::do_write() {
    auto self(shared_from_this());
    boost::asio::async_write(_socket,
                             boost::asio::buffer(_writeMsgQ.front().data(),
                                                 _writeMsgQ.front().length()),
                             [this, self](boost::system::error_code ec, std::size_t /*length*/) {
                                 if (!ec) {
                                     _writeMsgQ.pop_front();
                                     if (!_writeMsgQ.empty()) {
                                         do_write();
                                     }
                                 } else {
                                     _room.leave(shared_from_this());
                                 }
                             });
}
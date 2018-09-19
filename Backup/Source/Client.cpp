//
// Created by entropy on 9/18/18.
//

#include "Client.h"

Client::Client(const std::string &username,
                    boost::asio::io_service &io_service,
                       tcp::resolver::iterator endpoint_iterator)
        : _username(username),
        _io_service(io_service),
          _socket(io_service) {
    do_connect(endpoint_iterator);
}

void Client::write(const Packet &msg) {
    _io_service.post(
            [this, msg]() {
                bool write_in_progress = !_writeMsgQ.empty();
                _writeMsgQ.push_back(msg);
                if (!write_in_progress) {
                    do_write();
                }
            });
}

void Client::close() {
    _io_service.post([this]() { _socket.close(); });
}

void Client::do_connect(tcp::resolver::iterator endpoint_iterator) {
    boost::asio::async_connect(_socket, endpoint_iterator,
                               [this](boost::system::error_code ec, tcp::resolver::iterator) {
                                   if (!ec) {
                                       do_read_header();
                                   }
                               });
}

void Client::do_read_header() {
    boost::asio::async_read(_socket,
                            boost::asio::buffer(_readMsg.data(), Packet::header_length),
                            [this](boost::system::error_code ec, std::size_t /*length*/) {
                                if (!ec && _readMsg.decodeHeader()) {
                                    do_read_body();
                                } else {
                                    _socket.close();
                                }
                            });
}

void Client::do_read_body() {
    boost::asio::async_read(_socket,
                            boost::asio::buffer(_readMsg.body(), _readMsg.bodyLength()),
                            [this](boost::system::error_code ec, std::size_t /*length*/) {
                                if (!ec) {
                                    std::cout.write(_readMsg.body(), _readMsg.bodyLength());
                                    std::cout << "\n";
                                    do_read_header();
                                } else {
                                    _socket.close();
                                }
                            });
}

void Client::do_write() {
    boost::asio::async_write(_socket,
                             boost::asio::buffer(_writeMsgQ.front().data(),
                                                 _writeMsgQ.front().length()),
                             [this](boost::system::error_code ec, std::size_t /*length*/) {
                                 if (!ec) {
                                     _writeMsgQ.pop_front();
                                     if (!_writeMsgQ.empty()) {
                                         do_write();
                                     }
                                 } else {
                                     _socket.close();
                                 }
                             });
}

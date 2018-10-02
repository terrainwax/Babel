//
// Created by entropy on 10/1/18.
//

#include "Session.h"

Session::Session(boost::asio::io_context &io_context) : _socket(io_context), _active(false), _secured(false)
{

}

void Session::open()
{
    _active = true;
}

void Session::close()
{
    _active = false;
    _socket.close();
}

bool Session::isActive() const
{
    return _active;
}

bool Session::isSecured() const
{
    return _secured;
}

tcp::socket &Session::getSocket()
{
    return _socket;
}

BabelString Session::getAddress() const
{
    return BabelString(_socket.remote_endpoint().address().to_string().c_str());
}

std::ostream& operator<<(std::ostream& os, const Session& session)
{
    os << session.getAddress();

    return os;
}

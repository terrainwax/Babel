//
// Created by entropy on 10/1/18.
//

#ifndef CPP_BABEL_2018_SESSION_H
#define CPP_BABEL_2018_SESSION_H

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/algorithm/string.hpp>

#include "BabelString.h"

using boost::asio::ip::tcp;

class Session {
public:
    Session(boost::asio::io_context &io_context);
    tcp::socket &getSocket();
    BabelString getAddress() const;
protected:
    tcp::socket _socket;
    bool _secured;
};

std::ostream& operator<<(std::ostream& os, const Session& session);

#endif //CPP_BABEL_2018_SESSION_H

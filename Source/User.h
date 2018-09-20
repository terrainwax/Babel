//
// Created by entropy on 9/18/18.
//

#ifndef CPP_BABEL_2018_USER_H
#define CPP_BABEL_2018_USER_H

#include <set>
#include <string>
#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "Packet.h"

class Session;
typedef boost::shared_ptr<Session> SessionPointer;

class User {

public:
    void addSession(SessionPointer session);
    void removeSession(SessionPointer session);
    void transmit(SessionPointer participant, const Packet &msg);

private:
    std::set<SessionPointer> _sessions;
    enum {
        max_recent_msgs = 100
    };
    Message_queue _recentMsgQ;

    std::string _name;
    std::string _address;
};

#include "Session.h"

#endif //CPP_BABEL_2018_USER_H

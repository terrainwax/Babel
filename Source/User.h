//
// Created by entropy on 9/18/18.
//

#ifndef CPP_BABEL_2018_USER_H
#define CPP_BABEL_2018_USER_H

#include <set>
#include <string>
#include <iostream>
#include <deque>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

class Session;
typedef boost::shared_ptr<Session> SessionPointer;

class Message;
typedef std::deque<Message> MessageQueue;

class User {

public:
    User();

    void setName(const std::string &name);
    void addSession(SessionPointer session);
    void removeSession(SessionPointer session);
    void transmit(const Message &message);

private:
    std::set<SessionPointer> _sessions;
    enum {
        max_recent_messages = 100
    };
    MessageQueue _recentMessageQueue;

    std::string _name;
};

#include "Session.h"
#include "Message.h"

#endif //CPP_BABEL_2018_USER_H

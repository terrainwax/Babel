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

class Server;

class Session;
typedef boost::shared_ptr<Session> SessionPointer;

class Message;
typedef std::deque<Message> MessageQueue;

class User : public boost::enable_shared_from_this<User> {

public:
    User();
    typedef boost::shared_ptr<User> UserPointer;

    static UserPointer create(Server &_server, const std::string &name);

    void setName(const std::string &name);
    void addSession(SessionPointer session);
    void removeSession(SessionPointer session);
    void transmit(const Message &message);
    std::string getName();

private:
    explicit User(Server &_server, const std::string &name);

    std::set<SessionPointer> _sessions;
    enum {
        max_recent_messages = 100
    };
    MessageQueue _recentMessageQueue;

    Server &_server;
    std::string _name;
};

#include "Server.h"
#include "Session.h"
#include "Message.h"

#endif //CPP_BABEL_2018_USER_H

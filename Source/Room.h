//
// Created by entropy on 9/18/18.
//

#ifndef CPP_BABEL_2018_ROOM_H
#define CPP_BABEL_2018_ROOM_H

#include <set>
#include <memory>
#include <iostream>
#include "Message.h"

class User;

class Room {
public:
    void join(User *user);
    void leave(User *user);
    void shout(Message *message);
    void whisper(Message *message, User *user);

private:
    enum {
        maximum_recent_messages = 100
    };

    unsigned char _number = 0;
    std::set<User*> _members;
    MessageQueue _recentMessageQueue;
};

#include "User.h"

#endif //CPP_BABEL_2018_ROOM_H

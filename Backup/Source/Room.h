//
// Created by entropy on 9/18/18.
//

#ifndef CPP_BABEL_2018_ROOM_H
#define CPP_BABEL_2018_ROOM_H

#include <set>
#include <iostream>
#include "Participant.h"

class Room {
public:
    void join(Participant_ptr participant);

    void leave(Participant_ptr participant);

    void transmit(Participant_ptr participant, const Packet &msg);

private:
    std::set<Participant_ptr> _participants;
    enum {
        max_recent_msgs = 100
    };
    Message_queue _recentMsgQ;
};

#endif //CPP_BABEL_2018_ROOM_H

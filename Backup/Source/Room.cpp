//
// Created by entropy on 9/18/18.
//

#include "Room.h"

void Room::join(Participant_ptr participant) {
    std::cout << "Client joined room: " << participant->getName() << std::endl;
    _participants.insert(participant);
    for (auto msg: _recentMsgQ)
        participant->deliver(msg);
}

void Room::leave(Participant_ptr participant) {
    std::cout << "Client exited room: " << participant->getName() << std::endl;
    _participants.erase(participant);
}

void Room::transmit(Participant_ptr participant, const Packet &msg) {
    std::cout << participant->getName() << ": " << std::string(msg.body(), msg.bodyLength()) << std::endl;
    _recentMsgQ.push_back(msg);
    while (_recentMsgQ.size() > max_recent_msgs)
        _recentMsgQ.pop_front();

    for (auto participant: _participants)
        participant->deliver(msg);
}

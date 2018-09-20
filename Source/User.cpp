//
// Created by entropy on 9/18/18.
//

#include "User.h"

void User::addSession(SessionPointer session) {
    std::cout << "Session added." << std::endl;
    _sessions.insert(session);
    for (auto msg: _recentMsgQ)
        session->deliver(msg);
}

void User::removeSession(SessionPointer session) {
    std::cout << "Session removed." << std::endl;
    _sessions.erase(session);
}

void User::transmit(SessionPointer session, const Packet &msg) {
    std::cout << "Transmitting: " << std::string(msg.body(), msg.bodyLength()) << std::endl;
    _recentMsgQ.push_back(msg);
    while (_recentMsgQ.size() > max_recent_msgs)
        _recentMsgQ.pop_front();

    for (auto session: _sessions)
        session->deliver(msg);
}
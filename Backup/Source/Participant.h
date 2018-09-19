//
// Created by entropy on 9/18/18.
//

#ifndef CPP_BABEL_2018_PARTICIPANT_H
#define CPP_BABEL_2018_PARTICIPANT_H

#include <memory>
#include "Packet.h"

class Participant {
public:
    virtual ~Participant() {}

    virtual void deliver(const Packet &msg) = 0;
    virtual std::string getName() = 0;
};

typedef std::shared_ptr<Participant> Participant_ptr;

#endif //CPP_BABEL_2018_PARTICIPANT_H

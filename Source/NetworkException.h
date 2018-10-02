//
// Created by entropy on 9/28/18.
//

#ifndef CPP_BABEL_2018_NETWORK_EXCEPTION_H
#define CPP_BABEL_2018_NETWORK_EXCEPTION_H

#include "BabelException.h"

class NetworkException: public BabelException {
public:
    NetworkException(const BabelString &message);
};


#endif //CPP_BABEL_2018_NETWORK_EXCEPTION_H

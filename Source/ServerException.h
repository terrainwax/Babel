//
// Created by entropy on 9/28/18.
//

#ifndef CPP_BABEL_2018_CLIENT_EXCEPTION_H
#define CPP_BABEL_2018_CLIENT_EXCEPTION_H

#include "BabelException.h"

class ServerException: public BabelException {
public:
    ServerException(const BabelString &message);
};

#endif //CPP_BABEL_2018_CLIENT_EXCEPTION_H

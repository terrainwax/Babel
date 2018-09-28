//
// Created by entropy on 9/28/18.
//

#ifndef CPP_BABEL_2018_BABEL_EXCEPTION_H
#define CPP_BABEL_2018_BABEL_EXCEPTION_H

#include <exception>

#include "BabelString.h"

class BabelException: public std::exception {
public:
    BabelException(const BabelString &message);
    const char *what() const throw();

private:
    BabelString _message;
};

#endif //CPP_BABEL_2018_BABEL_EXCEPTION_H

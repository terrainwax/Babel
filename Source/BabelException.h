//
// Created by entropy on 9/28/18.
//

#ifndef CPP_BABEL_2018_BABEL_EXCEPTION_H
#define CPP_BABEL_2018_BABEL_EXCEPTION_H

#include <exception>
#include <string>

class BabelException: public std::exception {
public:
    BabelException(const std::string &message);
    const char *what() const throw();

private:
    std::string _message;
};

#endif //CPP_BABEL_2018_BABEL_EXCEPTION_H

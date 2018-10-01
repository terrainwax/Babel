//
// Created by entropy on 10/1/18.
//

#ifndef CPP_BABEL_2018_LOGGER_H
#define CPP_BABEL_2018_LOGGER_H

#include <iostream>
#include "BabelString.h"

class Logger {
public:
    void debug(const BabelString &message);
    void info(const BabelString &message);
    void warn(const BabelString &message);
    void error(const BabelString &message);

    static Logger* get();
private:
    Logger();
    Logger(const Logger &logger);
    Logger& operator=(const Logger&);

    static Logger *_instance;
};


#endif //CPP_BABEL_2018_LOGGER_H

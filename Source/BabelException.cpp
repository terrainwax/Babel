//
// Created by entropy on 9/28/18.
//

#include "BabelException.h"

BabelException::BabelException(const std::string &message)
    :   _message(message)
{

}

const char* BabelException::what() const throw()
{
    return _message.c_str();
}

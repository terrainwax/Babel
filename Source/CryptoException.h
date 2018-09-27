//
// Created by entropy on 9/28/18.
//

#ifndef CPP_BABEL_2018_CRYPTO_EXCEPTION_H
#define CPP_BABEL_2018_CRYPTO_EXCEPTION_H

#include "BabelException.h"

class CryptoException: public BabelException {
public:
    CryptoException(const std::string &message);
};


#endif //CPP_BABEL_2018_CRYPTO_EXCEPTION_H

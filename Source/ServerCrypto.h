/*
** EPITECH PROJECT, 2018
** CPP_babel_2018
** File description:
** ServerCrypto.hpp
*/

#ifndef CPP_BABEL_2018_SERVER_CRYPTO_HPP
#define CPP_BABEL_2018_SERVER_CRYPTO_HPP

#include "CommonCrypto.h"

class ServerCrypto: public CommonCrypto {
  public:
    ServerCrypto();
    ~ServerCrypto();

    BabelString decryptRSA(const BabelString &encryptedMessage);

    BabelString getLocalPublicKey();
    BabelString getLocalPrivateKey();

  private:
    RSA *_localKeyPairRSA;

    void initialize();
    void generateRsaKeyPair();
};

#endif

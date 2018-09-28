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

    BabelString decryptRSA(const BabelString &encryptedMessage, const BabelString &encryptionKey, const BabelString &encryptionIv);

    BabelString getLocalPublicKey();
    BabelString getLocalPrivateKey();

  private:
    EVP_PKEY *_localKeyPairRSA;

    EVP_CIPHER_CTX *_decryptContextRSA;

    void initialize();
    void generateRsaKeypair();
};

#endif

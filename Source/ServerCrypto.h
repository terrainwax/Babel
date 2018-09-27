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

    int decryptRSA(unsigned char *encryptedMessage, size_t encryptedMessageLength, unsigned char *encryptedKey,
                   size_t encryptedKeyLength,
                   unsigned char *iv, size_t ivLength, unsigned char **decryptedMessage);

    std::string getLocalPublicKey();
    std::string getLocalPrivateKey();

  private:
    EVP_PKEY *_localKeyPairRSA;

    EVP_CIPHER_CTX *_decryptContextRSA;

    int initialize();
    int generateRsaKeypair();
};

#endif

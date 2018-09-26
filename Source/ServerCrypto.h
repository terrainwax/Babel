/*
** EPITECH PROJECT, 2018
** CPP_babel_2018
** File description:
** ServerCrypto.hpp
*/

#ifndef CPP_BABEL_2018_SERVER_CRYPTO_HPP
#define CPP_BABEL_2018_SERVER_CRYPTO_HPP

#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/aes.h>
#include <openssl/err.h>
#include <openssl/rand.h>

#include <memory>
#include <string>
#include <cstring>

#define RSA_KEYLEN 2048

#define SUCCESS 0
#define FAILURE -1

class ServerCrypto {
  public:
    ServerCrypto();
    ~ServerCrypto();

    int decryptRSA(unsigned char *encryptedMessage, size_t encryptedMessageLength, unsigned char *encryptedKey,
                   size_t encryptedKeyLength,
                   unsigned char *iv, size_t ivLength, unsigned char **decryptedMessage);

    int encryptAES(const unsigned char *message, size_t messageLength, unsigned char **encryptedMessage);
    int decryptAES(unsigned char *encryptedMessage, size_t encryptedMessageLength, unsigned char **decryptedMessage);

    std::string getLocalPublicKey();
    std::string getLocalPrivateKey();

    int getAesKey(unsigned char **aesKey);
    int setAesKey(unsigned char *aesKey, size_t aesKeyLen);

    int getAesIv(unsigned char **aesIv);
    int setAesIv(unsigned char *aesIv, size_t aesIvLen);

  private:
    EVP_PKEY *_localKeyPairRSA;

    EVP_CIPHER_CTX *_decryptContextRSA;

    EVP_CIPHER_CTX *_encryptContextAES;
    EVP_CIPHER_CTX *_decryptContextAES;

    unsigned char *_aesKey;
    unsigned char *_aesIv;

    size_t aesKeyLength;
    size_t aesIvLength;

    int init();
    int generateRsaKeypair(EVP_PKEY **keypair);
    int bioToString(BIO *bio, unsigned char **string);
};

#endif

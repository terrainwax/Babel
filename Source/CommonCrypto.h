//
// Created by entropy on 9/28/18.
//

#ifndef CPP_BABEL_2018_COMMON_CRYPTO_H
#define CPP_BABEL_2018_COMMON_CRYPTO_H


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

class CommonCrypto {
public:
    CommonCrypto();
    ~CommonCrypto();

    int encryptAES(const unsigned char *message, size_t messageLength, unsigned char **encryptedMessage);
    int decryptAES(unsigned char *encryptedMessage, size_t encryptedMessageLength, unsigned char **decryptedMessage);

    std::string getAESKey();
    int setAESKey(const std::string &aesKey);

    std::string getAESIv();
    int setAESIv(const std::string &aesIv);

protected:
    std::string _aesKey;
    std::string _aesIv;

private:
    EVP_CIPHER_CTX *_encryptContextAES;
    EVP_CIPHER_CTX *_decryptContextAES;

    int initialize();
};


#endif //CPP_BABEL_2018_COMMON_CRYPTO_H

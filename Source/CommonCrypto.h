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

#include "CryptoException.h"

#define RSA_KEYLEN 2048

class CommonCrypto {
public:
    CommonCrypto();
    ~CommonCrypto();

    std::string encryptAES(const std::string &message);
    std::string decryptAES(const std::string &encryptedMessage);

    std::string getAESKey();
    void setAESKey(const std::string &aesKey);

    std::string getAESIv();
    void setAESIv(const std::string &aesIv);

protected:
    std::string _aesKey;
    std::string _aesIv;

private:
    EVP_CIPHER_CTX *_encryptContextAES;
    EVP_CIPHER_CTX *_decryptContextAES;

    void initialize();
};


#endif //CPP_BABEL_2018_COMMON_CRYPTO_H

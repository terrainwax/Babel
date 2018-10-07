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
#include <cstring>

#include "BabelString.h"
#include "CryptoException.h"

#define RSA_PADDING RSA_PKCS1_PADDING
#define RSA_KEYLEN 2048

class CommonCrypto {
public:
    CommonCrypto();
    ~CommonCrypto();

    BabelString encryptAES(const BabelString &message);
    BabelString decryptAES(const BabelString &encryptedMessage);

    BabelString getAESKey();
    void setAESKey(const BabelString &aesKey);

    BabelString getAESIv();
    void setAESIv(const BabelString &aesIv);

protected:
    BabelString _aesKey;
    BabelString _aesIv;

private:
    EVP_CIPHER_CTX *_encryptContextAES;
    EVP_CIPHER_CTX *_decryptContextAES;

    void initialize();
};


#endif //CPP_BABEL_2018_COMMON_CRYPTO_H

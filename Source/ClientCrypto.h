#ifndef CPP_BABEL_2018_CLIENT_CRYPTO_HPP
#define CPP_BABEL_2018_CLIENT_CRYPTO_HPP

#include "CommonCrypto.h"

class ClientCrypto: public CommonCrypto {
public:
    ClientCrypto();
    ~ClientCrypto();

    std::string encryptRSA(const std::string &message, std::string &encryptionKey, std::string &encryptionIv);

    std::string getRemotePublicKey();
    void setRemotePublicKey(const std::string &publicKey);

private:
    EVP_PKEY *_remotePublicKeyRSA;

    EVP_CIPHER_CTX *_encryptContextRSA;

    void initialize();
    void generateAesKey();
};

#endif

#ifndef CPP_BABEL_2018_CLIENT_CRYPTO_HPP
#define CPP_BABEL_2018_CLIENT_CRYPTO_HPP

#include "CommonCrypto.h"

class ClientCrypto: public CommonCrypto {
public:
    ClientCrypto();
    ~ClientCrypto();

    BabelString encryptRSA(const BabelString &message, BabelString &encryptionKey, BabelString &encryptionIv);

    BabelString getRemotePublicKey();
    void setRemotePublicKey(const BabelString &publicKey);

private:
    EVP_PKEY *_remotePublicKeyRSA;

    EVP_CIPHER_CTX *_encryptContextRSA;

    void initialize();
    void generateAesKey();
};

#endif

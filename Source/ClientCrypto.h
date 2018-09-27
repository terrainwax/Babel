#ifndef CPP_BABEL_2018_CLIENT_CRYPTO_HPP
#define CPP_BABEL_2018_CLIENT_CRYPTO_HPP

#include "CommonCrypto.h"

class ClientCrypto: public CommonCrypto {
public:
    ClientCrypto();
    ~ClientCrypto();

    int encryptRSA(const unsigned char *message, size_t messageLength, unsigned char **encryptedMessage,
                   unsigned char **encryptedKey,
                   size_t *encryptedKeyLength, unsigned char **iv, size_t *ivLength);

    std::string getRemotePublicKey();
    int setRemotePublicKey(const std::string &publicKey);

    int generateAesKey();

private:
    EVP_PKEY *_remotePublicKeyRSA;

    EVP_CIPHER_CTX *_encryptContextRSA;

    int initialize();
};

#endif

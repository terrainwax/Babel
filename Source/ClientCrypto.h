#ifndef CPP_BABEL_2018_CLIENT_CRYPTO_HPP
#define CPP_BABEL_2018_CLIENT_CRYPTO_HPP

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

class ClientCrypto {
public:
    ClientCrypto();
    ~ClientCrypto();

    int encryptRSA(const unsigned char *message, size_t messageLength, unsigned char **encryptedMessage,
                   unsigned char **encryptedKey,
                   size_t *encryptedKeyLength, unsigned char **iv, size_t *ivLength);

    int encryptAES(const unsigned char *message, size_t messageLength, unsigned char **encryptedMessage);
    int decryptAES(unsigned char *encryptedMessage, size_t encryptedMessageLength, unsigned char **decryptedMessage);

    int getRemotePublicKey(unsigned char **publicKey);
    int setRemotePublicKey(const std::string &publicKey);

    std::string getAESKey();
    int setAESKey(const std::string &aesKey);

    std::string getAESIv();
    int setAESIv(const std::string &aesIv);

    int generateAesKey();

private:
    EVP_PKEY *_remotePublicKeyRSA;

    EVP_CIPHER_CTX *_encryptContextRSA;

    EVP_CIPHER_CTX *_encryptContextAES;
    EVP_CIPHER_CTX *_decryptContextAES;

    std::string _aesKey;
    std::string _aesIv;

    int init();
    int bioToString(BIO *bio, unsigned char **string);
};

#endif

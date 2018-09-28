#include "ClientCrypto.h"

ClientCrypto::ClientCrypto()
: CommonCrypto()
{
    _remotePublicKeyRSA = NULL;

    initialize();
}

ClientCrypto::~ClientCrypto() {
    EVP_PKEY_free(_remotePublicKeyRSA);

    EVP_CIPHER_CTX_free(_encryptContextRSA);
}

void ClientCrypto::initialize() {
    _encryptContextRSA = EVP_CIPHER_CTX_new();

    if(_encryptContextRSA == NULL)
        throw CryptoException("Cannot Create RSA Encryption Context");

    generateAesKey();
}

void ClientCrypto::generateAesKey() {
    char aesKeyBuffer[_aesKey.size()];
    char aesIvBuffer[_aesIv.size()];

    if(RAND_bytes((unsigned char *)aesKeyBuffer, _aesKey.size()) == 0)
        throw CryptoException("Cannot Generate AES Key");

    if(RAND_bytes((unsigned char *)aesIvBuffer, _aesIv.size()) == 0)
        throw CryptoException("Cannot Generate AES Iv");

    _aesKey = std::string(aesKeyBuffer, _aesKey.size());
    _aesIv = std::string(aesIvBuffer, _aesIv.size());
}

#include <iostream>

std::string ClientCrypto::encryptRSA(const std::string &message, std::string &encryptionKey, std::string &encryptionIv) {
    size_t encryptedMessageLength = 0;
    size_t encryptionKeyLength = 0;
    size_t blockLength = 0;

    char encryptionKeyBuffer[EVP_PKEY_size(_remotePublicKeyRSA)];
    char encryptionIvBuffer[EVP_MAX_IV_LENGTH];

    char encryptedMessageBuffer[message.size() + EVP_MAX_IV_LENGTH];

    std::cout << "Jambon" << std::endl;

    if(!EVP_SealInit(_encryptContextRSA, EVP_aes_256_cbc(), (unsigned char **)&encryptionKeyBuffer, (int*)&encryptionKeyLength, (unsigned char *)encryptionIvBuffer, &_remotePublicKeyRSA, 1))
        throw CryptoException("Cannot Initialize RSA Encryption");

    std::cout << "Jambon: " <<  message << std::endl;

    if(!EVP_SealUpdate(_encryptContextRSA, (unsigned char *)encryptedMessageBuffer + encryptedMessageLength, (int*)&blockLength, (const unsigned char*)message.c_str(), (int)message.size()))
        throw CryptoException("Cannot Update RSA Encryption");

    std::cout << "Jambon" << std::endl;

    encryptedMessageLength += blockLength;

    if(!EVP_SealFinal(_encryptContextRSA, (unsigned char *)encryptedMessageBuffer + encryptedMessageLength, (int*)&blockLength))
        throw CryptoException("Cannot Finalize RSA Encryption");

    std::cout << "Jambon" << std::endl;

    encryptedMessageLength += blockLength;
    encryptionKey = std::string(encryptionKeyBuffer, encryptionKeyLength);
    encryptionIv = std::string(encryptionIvBuffer, EVP_MAX_IV_LENGTH);

    return std::string(encryptedMessageBuffer, encryptedMessageLength);
}

std::string ClientCrypto::getRemotePublicKey()
{
    BIO *bio = BIO_new(BIO_s_mem());

    PEM_write_bio_PUBKEY(bio, _remotePublicKeyRSA);
    int bioLength = BIO_pending(bio);

    char keyBuffer[bioLength];

    BIO_read(bio, keyBuffer, bioLength);
    BIO_free_all(bio);

    return std::string(keyBuffer, bioLength - 1);
}

void ClientCrypto::setRemotePublicKey(const std::string &publicKey) {
    BIO *bio = BIO_new(BIO_s_mem());

    BIO_write(bio, publicKey.data(), publicKey.size());
    PEM_read_bio_PUBKEY(bio, &_remotePublicKeyRSA, NULL, NULL);

    BIO_free_all(bio);
}

#include "ClientCrypto.h"

ClientCrypto::ClientCrypto() {
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
    if(RAND_bytes((unsigned char *)_aesKey.data(), _aesKey.size()) == 0)
        throw CryptoException("Cannot Generate AES Key");

    if(RAND_bytes((unsigned char *)_aesIv.data(), _aesIv.size()) == 0)
        throw CryptoException("Cannot Generate AES Iv");
}

std::string ClientCrypto::encryptRSA(const std::string &message, std::string &encryptionKey, std::string &encryptionIv) {
    size_t encryptedMessageLength = 0;
    size_t encryptionKeyLength = 0;
    size_t blockLength = 0;

    encryptionKey = std::string('\0', EVP_PKEY_size(_remotePublicKeyRSA));
    encryptionIv = std::string('\0', EVP_MAX_IV_LENGTH);

    char *encryptionKeyBuffer = encryptionKey.data();

    std::string encryptedMessage = std::string('\0', message.size() + EVP_MAX_IV_LENGTH);

    if(!EVP_SealInit(_encryptContextRSA, EVP_aes_256_cbc(), (unsigned char **)&encryptionKeyBuffer, (int*)&encryptionKeyLength, (unsigned char *)encryptionIv.data(), &_remotePublicKeyRSA, 1))
        throw CryptoException("Cannot Initialize RSA Encryption");

    encryptionKey = std::string(encryptionKeyBuffer, encryptionKeyLength);

    if(!EVP_SealUpdate(_encryptContextRSA, (unsigned char *)encryptedMessage.data() + encryptedMessageLength, (int*)&blockLength, (const unsigned char*)message.c_str(), (int)message.size()))
        throw CryptoException("Cannot Update RSA Encryption");

    encryptedMessageLength += blockLength;

    if(!EVP_SealFinal(_encryptContextRSA, (unsigned char *)encryptedMessage.data() + encryptedMessageLength, (int*)&blockLength))
        throw CryptoException("Cannot Finalize RSA Encryption");

    encryptedMessageLength += blockLength;
    encryptionKey = encryptionKey.substr(0, encryptionKeyLength);

    return encryptedMessage.substr(0, encryptedMessageLength);
}

std::string ClientCrypto::getRemotePublicKey()
{
    BIO *bio = BIO_new(BIO_s_mem());

    PEM_write_bio_PUBKEY(bio, _remotePublicKeyRSA);
    int bioLength = BIO_pending(bio);
    std::string key = std::string(bioLength, '\0');
    BIO_read(bio, key.data(), bioLength);
    key = key.substr(0, key.size() - 1);

    BIO_free_all(bio);

    return key;
}

void ClientCrypto::setRemotePublicKey(const std::string &publicKey) {
    BIO *bio = BIO_new(BIO_s_mem());

    BIO_write(bio, publicKey.data(), publicKey.size());
    PEM_read_bio_PUBKEY(bio, &_remotePublicKeyRSA, NULL, NULL);

    BIO_free_all(bio);
}

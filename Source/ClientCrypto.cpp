#include "ClientCrypto.h"

ClientCrypto::ClientCrypto() {
    _remotePublicKeyRSA = NULL;

    initialize();
}

ClientCrypto::~ClientCrypto() {
    EVP_PKEY_free(_remotePublicKeyRSA);

    EVP_CIPHER_CTX_free(_encryptContextRSA);
}

int ClientCrypto::initialize() {
    _encryptContextRSA = EVP_CIPHER_CTX_new();

    // Check if any of the contexts initializations failed
    if(_encryptContextRSA == NULL) {
        return FAILURE;
    }

    return generateAesKey();
}

int ClientCrypto::generateAesKey() {
    if(RAND_bytes((unsigned char *)_aesKey.data(), _aesKey.size()) == 0) {
        return FAILURE;
    }

    if(RAND_bytes((unsigned char *)_aesIv.data(), _aesIv.size()) == 0) {
        return FAILURE;
    }

    return SUCCESS;
}

int ClientCrypto::encryptRSA(const unsigned char *message, size_t messageLength, unsigned char **encryptedMessage,
                             unsigned char **encryptedKey,
                             size_t *encryptedKeyLength, unsigned char **iv, size_t *ivLength) {

    // Allocate memory for everything
    size_t encryptedMessageLength = 0;
    size_t blockLength = 0;

    *encryptedKey = (unsigned char*)malloc(EVP_PKEY_size(_remotePublicKeyRSA));
    *iv = (unsigned char*)malloc(EVP_MAX_IV_LENGTH);
    *ivLength = EVP_MAX_IV_LENGTH;

    if(*encryptedKey == NULL || *iv == NULL) {
        return FAILURE;
    }

    *encryptedMessage = (unsigned char*)malloc(messageLength + EVP_MAX_IV_LENGTH);
    if(encryptedMessage == NULL) {
        return FAILURE;
    }

    // Encrypt it!
    if(!EVP_SealInit(_encryptContextRSA, EVP_aes_256_cbc(), encryptedKey, (int*)encryptedKeyLength, *iv, &_remotePublicKeyRSA, 1)) {
        return FAILURE;
    }

    if(!EVP_SealUpdate(_encryptContextRSA, *encryptedMessage + encryptedMessageLength, (int*)&blockLength, (const unsigned char*)message, (int)messageLength)) {
        return FAILURE;
    }
    encryptedMessageLength += blockLength;

    if(!EVP_SealFinal(_encryptContextRSA, *encryptedMessage + encryptedMessageLength, (int*)&blockLength)) {
        return FAILURE;
    }
    encryptedMessageLength += blockLength;

    return (int)encryptedMessageLength;
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

int ClientCrypto::setRemotePublicKey(const std::string &publicKey) {
    BIO *bio = BIO_new(BIO_s_mem());

    BIO_write(bio, publicKey.data(), publicKey.size());
    PEM_read_bio_PUBKEY(bio, &_remotePublicKeyRSA, NULL, NULL);

    BIO_free_all(bio);

    return SUCCESS;
}

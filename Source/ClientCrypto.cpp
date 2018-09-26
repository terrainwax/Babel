#include "ClientCrypto.h"

ClientCrypto::ClientCrypto() {
    _remotePublicKeyRSA = NULL;

    init();
}

ClientCrypto::~ClientCrypto() {
    EVP_PKEY_free(_remotePublicKeyRSA);

    EVP_CIPHER_CTX_free(_encryptContextRSA);

    EVP_CIPHER_CTX_free(_encryptContextAES);
    EVP_CIPHER_CTX_free(_decryptContextAES);

    free(_aesKey);
    free(_aesIv);
}

int ClientCrypto::init() {
    _encryptContextRSA = EVP_CIPHER_CTX_new();

    _encryptContextAES = EVP_CIPHER_CTX_new();
    _decryptContextAES = EVP_CIPHER_CTX_new();

    // Check if any of the contexts initializations failed
    if(_encryptContextRSA == NULL || _encryptContextAES == NULL || _decryptContextAES == NULL) {
        return FAILURE;
    }

    /* Don't set key or IV right away; we want to set lengths */
    EVP_CIPHER_CTX_init(_encryptContextAES);
    EVP_CIPHER_CTX_init(_decryptContextAES);

    EVP_CipherInit_ex(_encryptContextAES, EVP_aes_256_cbc(), NULL, NULL, NULL, 1);

    /* Now we can set key and IV lengths */
    aesKeyLength = EVP_CIPHER_CTX_key_length(_encryptContextAES);
    aesIvLength = EVP_CIPHER_CTX_iv_length(_encryptContextAES);

    // Generate RSA and AES keys
    generateAesKey(&_aesKey, &_aesIv);

    return SUCCESS;
}

int ClientCrypto::generateAesKey(unsigned char **aesKey, unsigned char **aesIv) {
    *aesKey = (unsigned char*)malloc(aesKeyLength);
    *aesIv = (unsigned char*)malloc(aesIvLength);

    if(aesKey == NULL || aesIv == NULL) {
        return FAILURE;
    }

    if(RAND_bytes(*aesKey, aesKeyLength) == 0) {
        return FAILURE;
    }

    if(RAND_bytes(*aesIv, aesIvLength) == 0) {
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

int ClientCrypto::encryptAES(const unsigned char *message, size_t messageLength, unsigned char **encryptedMessage) {
    // Allocate memory for everything
    size_t blockLength = 0;
    size_t encryptedMessageLength = 0;

    *encryptedMessage = (unsigned char*)malloc(messageLength + AES_BLOCK_SIZE);
    if(encryptedMessage == NULL) {
        return FAILURE;
    }

    // Encrypt it!
    if(!EVP_EncryptInit_ex(_encryptContextAES, EVP_aes_256_cbc(), NULL, _aesKey, _aesIv)) {
        return FAILURE;
    }

    if(!EVP_EncryptUpdate(_encryptContextAES, *encryptedMessage, (int*)&blockLength, (unsigned char*)message, messageLength)) {
        return FAILURE;
    }
    encryptedMessageLength += blockLength;

    if(!EVP_EncryptFinal_ex(_encryptContextAES, *encryptedMessage + encryptedMessageLength, (int*)&blockLength)) {
        return FAILURE;
    }

    return encryptedMessageLength + blockLength;
}

int ClientCrypto::decryptAES(unsigned char *encryptedMessage, size_t encryptedMessageLength,
                             unsigned char **decryptedMessage) {
    // Allocate memory for everything
    size_t decryptedMessageLength = 0;
    size_t blockLength = 0;

    *decryptedMessage = (unsigned char*)malloc(encryptedMessageLength);
    if(*decryptedMessage == NULL) {
        return FAILURE;
    }

    // Decrypt it!
    if(!EVP_DecryptInit_ex(_decryptContextAES, EVP_aes_256_cbc(), NULL, _aesKey, _aesIv)) {
        return FAILURE;
    }

    if(!EVP_DecryptUpdate(_decryptContextAES, (unsigned char*)*decryptedMessage, (int*)&blockLength, encryptedMessage, (int)encryptedMessageLength)) {
        return FAILURE;
    }
    decryptedMessageLength += blockLength;

    if(!EVP_DecryptFinal_ex(_decryptContextAES, (unsigned char*)*decryptedMessage + decryptedMessageLength, (int*)&blockLength)) {
        return FAILURE;
    }
    decryptedMessageLength += blockLength;

    return (int)decryptedMessageLength;
}

int ClientCrypto::getRemotePublicKey(unsigned char **publicKey) {
    BIO *bio = BIO_new(BIO_s_mem());
    PEM_write_bio_PUBKEY(bio, _remotePublicKeyRSA);
    return bioToString(bio, publicKey);
}

int ClientCrypto::setRemotePublicKey(const std::string &publicKey) {
    BIO *bio = BIO_new(BIO_s_mem());

    BIO_write(bio, publicKey.data(), publicKey.size());
    PEM_read_bio_PUBKEY(bio, &_remotePublicKeyRSA, NULL, NULL);

    BIO_free_all(bio);

    return SUCCESS;
}

int ClientCrypto::getAesKey(unsigned char **aesKey) {
    *aesKey = this->_aesKey;
    return aesKeyLength;
}

int ClientCrypto::setAesKey(unsigned char *aesKey, size_t aesKeyLengthgth) {
    // Ensure the new key is the proper size
    if(aesKeyLengthgth != aesKeyLength) {
        return FAILURE;
    }

    std::memcpy(this->_aesKey, aesKey, aesKeyLength);

    return SUCCESS;
}

int ClientCrypto::getAesIv(unsigned char **aesIv) {
    *aesIv = this->_aesIv;
    return aesIvLength;
}

int ClientCrypto::setAesIv(unsigned char *aesIv, size_t aesIvLengthgth) {
    // Ensure the new IV is the proper size
    if(aesIvLengthgth != aesIvLength) {
        return FAILURE;
    }

    std::memcpy(this->_aesIv, aesIv, aesIvLength);

    return SUCCESS;
}

int ClientCrypto::bioToString(BIO *bio, unsigned char **string) {
    size_t bioLength = BIO_pending(bio);
    *string = (unsigned char*)malloc(bioLength + 1);

    if(string == NULL) {
        return FAILURE;
    }

    BIO_read(bio, *string, bioLength);

    // Insert the NUL terminator
    (*string)[bioLength] = '\0';

    BIO_free_all(bio);

    return (int)bioLength;
}

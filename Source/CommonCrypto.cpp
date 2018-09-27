#include "CommonCrypto.h"

CommonCrypto::CommonCrypto() {
    initialize();
}

CommonCrypto::~CommonCrypto() {
    EVP_CIPHER_CTX_free(_encryptContextAES);
    EVP_CIPHER_CTX_free(_decryptContextAES);
}

int CommonCrypto::initialize() {
    _encryptContextAES = EVP_CIPHER_CTX_new();
    _decryptContextAES = EVP_CIPHER_CTX_new();

    // Check if any of the contexts initializations failed
    if(_encryptContextAES == NULL || _decryptContextAES == NULL) {
        return FAILURE;
    }

    /* Don't set key or IV right away; we want to set lengths */
    EVP_CIPHER_CTX_init(_encryptContextAES);
    EVP_CIPHER_CTX_init(_decryptContextAES);

    EVP_CipherInit_ex(_encryptContextAES, EVP_aes_256_cbc(), NULL, NULL, NULL, 1);

    /* Now we can set key and IV lengths */
    _aesKey = std::string('\0', EVP_CIPHER_CTX_key_length(_encryptContextAES));
    _aesIv = std::string('\0', EVP_CIPHER_CTX_iv_length(_encryptContextAES));

    return SUCCESS;
}

int CommonCrypto::encryptAES(const unsigned char *message, size_t messageLength, unsigned char **encryptedMessage) {
    // Allocate memory for everything
    size_t blockLength = 0;
    size_t encryptedMessageLength = 0;

    *encryptedMessage = (unsigned char*)malloc(messageLength + AES_BLOCK_SIZE);
    if(encryptedMessage == NULL) {
        return FAILURE;
    }

    // Encrypt it!
    if(!EVP_EncryptInit_ex(_encryptContextAES, EVP_aes_256_cbc(), NULL, (unsigned char *)_aesKey.c_str(), (unsigned char *)_aesIv.c_str())) {
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

int CommonCrypto::decryptAES(unsigned char *encryptedMessage, size_t encryptedMessageLength,
                             unsigned char **decryptedMessage) {
    // Allocate memory for everything
    size_t decryptedMessageLength = 0;
    size_t blockLength = 0;

    *decryptedMessage = (unsigned char*)malloc(encryptedMessageLength);
    if(*decryptedMessage == NULL) {
        return FAILURE;
    }

    // Decrypt it!
    if(!EVP_DecryptInit_ex(_decryptContextAES, EVP_aes_256_cbc(), NULL, (unsigned char *)_aesKey.c_str(), (unsigned char *)_aesIv.c_str())) {
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

std::string CommonCrypto::getAESKey()
{
    return _aesKey;
}

int CommonCrypto::setAESKey(const std::string &aesKey)
{
    if (_aesKey.size() != aesKey.size())
        return FAILURE;

    _aesKey = aesKey;

    return SUCCESS;
}

std::string CommonCrypto::getAESIv()
{
    return _aesIv;
}

int CommonCrypto::setAESIv(const std::string &aesIv)
{
    if (_aesIv.size() != aesIv.size())
        return FAILURE;

    _aesIv = aesIv;

    return SUCCESS;
}

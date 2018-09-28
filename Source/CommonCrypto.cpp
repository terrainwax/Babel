#include "CommonCrypto.h"

CommonCrypto::CommonCrypto()
{
    initialize();
}

CommonCrypto::~CommonCrypto()
{
    EVP_CIPHER_CTX_free(_encryptContextAES);
    EVP_CIPHER_CTX_free(_decryptContextAES);
}

void CommonCrypto::initialize()
{
    _encryptContextAES = EVP_CIPHER_CTX_new();
    _decryptContextAES = EVP_CIPHER_CTX_new();

    if(_encryptContextAES == NULL || _decryptContextAES == NULL)
        throw CryptoException("Cannot Create AES Encryption & Decryption Context");

    EVP_CIPHER_CTX_init(_encryptContextAES);
    EVP_CIPHER_CTX_init(_decryptContextAES);

    EVP_CipherInit_ex(_encryptContextAES, EVP_aes_256_cbc(), NULL, NULL, NULL, 1);

    _aesKey = std::string('A', EVP_CIPHER_CTX_key_length(_encryptContextAES));
    _aesIv = std::string('A', EVP_CIPHER_CTX_iv_length(_encryptContextAES));
}

std::string CommonCrypto::encryptAES(const std::string &message)
{
    size_t blockLength = 0;
    size_t encryptedMessageLength;

    char encryptedMessageBuffer[message.size() + AES_BLOCK_SIZE];

    if(!EVP_EncryptInit_ex(_encryptContextAES, EVP_aes_256_cbc(), NULL, (unsigned char *)_aesKey.c_str(), (unsigned char *)_aesIv.c_str()))
        throw CryptoException("Cannot Initialize AES Encryption");

    if(!EVP_EncryptUpdate(_encryptContextAES, (unsigned char *)encryptedMessageBuffer, (int*)&blockLength, (unsigned char *)message.c_str(), message.size()))
        throw CryptoException("Cannot Update AES Encryption");

    encryptedMessageLength += blockLength;

    if(!EVP_EncryptFinal_ex(_encryptContextAES, (unsigned char *)encryptedMessageBuffer + encryptedMessageLength, (int*)&blockLength))
        throw CryptoException("Cannot Finalize AES Encryption");

    encryptedMessageLength += blockLength;

    return std::string(encryptedMessageBuffer, encryptedMessageLength);
}

std::string CommonCrypto::decryptAES(const std::string &encryptedMessage) {
    size_t blockLength = 0;
    size_t decryptedMessageLength = 0;

    char decryptedMessageBuffer[encryptedMessage.size()];

    if(!EVP_DecryptInit_ex(_decryptContextAES, EVP_aes_256_cbc(), NULL, (unsigned char *)_aesKey.c_str(), (unsigned char *)_aesIv.c_str()))
        throw CryptoException("Cannot Initialize AES Decryption");

    if(!EVP_DecryptUpdate(_decryptContextAES, (unsigned char*)decryptedMessageBuffer, (int*)&blockLength, (unsigned char *)encryptedMessage.c_str(), encryptedMessage.size()))
        throw CryptoException("Cannot Update AES Decryption");

    decryptedMessageLength += blockLength;

    if(!EVP_DecryptFinal_ex(_decryptContextAES, (unsigned char*)decryptedMessageBuffer + decryptedMessageLength, (int*)&blockLength))
        throw CryptoException("Cannot Finalize AES Decryption");

    decryptedMessageLength += blockLength;

    return std::string(decryptedMessageBuffer, decryptedMessageLength);
}

std::string CommonCrypto::getAESKey()
{
    return _aesKey;
}

void CommonCrypto::setAESKey(const std::string &aesKey)
{
    if (_aesKey.size() != aesKey.size())
        throw CryptoException("Cannot Set New AES Key : Wrong Size");

    _aesKey = aesKey;
}

std::string CommonCrypto::getAESIv()
{
    return _aesIv;
}

void CommonCrypto::setAESIv(const std::string &aesIv)
{
    if (_aesIv.size() != aesIv.size())
        throw CryptoException("Cannot Set New AES Iv : Wrong Size");

    _aesIv = aesIv;
}

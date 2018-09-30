#include "ClientCrypto.h"

ClientCrypto::ClientCrypto()
: CommonCrypto()
{
    _remotePublicKeyRSA = NULL;

    initialize();
}

ClientCrypto::~ClientCrypto() {
    RSA_free(_remotePublicKeyRSA);
}

void ClientCrypto::initialize() {
    generateAesKey();
}

void ClientCrypto::generateAesKey() {
    if(RAND_bytes((unsigned char *)_aesKey.getData(), _aesKey.getSize()) == 0)
        throw CryptoException("Cannot Generate AES Key");

    if(RAND_bytes((unsigned char *)_aesIv.getData(), _aesIv.getSize()) == 0)
        throw CryptoException("Cannot Generate AES Iv");
}

BabelString ClientCrypto::encryptRSA(const BabelString &message) {
    char encryptedMessageBuffer[4098];
    int encryptedMessageLength = 0;

    encryptedMessageLength = RSA_public_encrypt(message.getSize(), (unsigned char *)message.getData(), (unsigned char *)encryptedMessageBuffer, _remotePublicKeyRSA, RSA_PADDING);

    if (encryptedMessageLength <= 0)
        throw CryptoException("RSA Encryption Failed");

    return BabelString(encryptedMessageBuffer, encryptedMessageLength);
}

BabelString ClientCrypto::getRemotePublicKey()
{
    BIO *bio = BIO_new(BIO_s_mem());

    PEM_write_bio_RSAPublicKey(bio, _remotePublicKeyRSA);
    int bioLength = BIO_pending(bio);

    char keyBuffer[bioLength];

    BIO_read(bio, keyBuffer, bioLength);
    BIO_free_all(bio);

    return BabelString(keyBuffer, bioLength - 1);
}

void ClientCrypto::setRemotePublicKey(const BabelString &publicKey) {
    BIO *bio = BIO_new(BIO_s_mem());

    BIO_write(bio, publicKey.getData(), publicKey.getSize());
    PEM_read_bio_RSAPublicKey(bio, &_remotePublicKeyRSA, NULL, NULL);

    BIO_free_all(bio);
}

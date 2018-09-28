/*
** EPITECH PROJECT, 2018
** CPP_babel_2018
** File description:
** Main.cpp
*/

#include "ServerCrypto.h"

ServerCrypto::ServerCrypto()
: CommonCrypto()
{
	_localKeyPairRSA = NULL;

	initialize();
}

ServerCrypto::~ServerCrypto()
{
	EVP_CIPHER_CTX_free(_decryptContextRSA);
}

void ServerCrypto::initialize()
{
	_decryptContextRSA = EVP_CIPHER_CTX_new();

	if (_decryptContextRSA == NULL)
		throw CryptoException("Cannot Create RSA Decryption Context");

	generateRsaKeypair();
}

void ServerCrypto::generateRsaKeypair()
{
	EVP_PKEY_CTX *context = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);

	if (EVP_PKEY_keygen_init(context) <= 0)
		throw CryptoException("Cannot Generate RSA KeyPair");

	if (EVP_PKEY_CTX_set_rsa_keygen_bits(context, RSA_KEYLEN) <= 0)
		throw CryptoException("Cannot Generate RSA KeyPair");

	if (EVP_PKEY_keygen(context, &_localKeyPairRSA) <= 0)
		throw CryptoException("Cannot Generate RSA KeyPair");

	EVP_PKEY_CTX_free(context);
}

std::string ServerCrypto::decryptRSA(const std::string &encryptedMessage, const std::string &encryptionKey, const std::string &encryptionIv)
{
	size_t decryptedMessageLength = 0;
	size_t blockLength = 0;

	char decryptedMessageBuffer[encryptedMessage.size() + encryptionIv.size()];

	if (!EVP_OpenInit(_decryptContextRSA, EVP_aes_256_cbc(), (unsigned char *)encryptionKey.data(),
		encryptionKey.size(), (unsigned char *)encryptionIv.data(), _localKeyPairRSA))
		throw CryptoException("Cannot Initialize RSA Decryption");

	if (!EVP_OpenUpdate(_decryptContextRSA,
		(unsigned char *)decryptedMessageBuffer + decryptedMessageLength,
		(int *)&blockLength, (unsigned char *)encryptedMessage.c_str(),
		(int)encryptedMessage.size()))
		throw CryptoException("Cannot Update RSA Decryption");

	decryptedMessageLength += blockLength;

	if (!EVP_OpenFinal(_decryptContextRSA,
		(unsigned char *)decryptedMessageBuffer + decryptedMessageLength,
		(int *)&blockLength))
		throw CryptoException("Cannot Finalize RSA Decryption");

	decryptedMessageLength += blockLength;

	return std::string(decryptedMessageBuffer, decryptedMessageLength);
}

std::string ServerCrypto::getLocalPublicKey()
{
    BIO *bio = BIO_new(BIO_s_mem());

    PEM_write_bio_PUBKEY(bio, _localKeyPairRSA);
    int bioLength = BIO_pending(bio);
    char keyBuffer[bioLength];

    BIO_read(bio, keyBuffer, bioLength);
    BIO_free_all(bio);

    return std::string(keyBuffer, bioLength - 1);
}

std::string ServerCrypto::getLocalPrivateKey()
{
	BIO *bio = BIO_new(BIO_s_mem());

	PEM_write_bio_PrivateKey(bio, _localKeyPairRSA, NULL, NULL, 0, 0, NULL);
	int bioLength = BIO_pending(bio);
    char keyBuffer[bioLength];

    BIO_read(bio, keyBuffer, bioLength);
    BIO_free_all(bio);

    return std::string(keyBuffer, bioLength - 1);
}

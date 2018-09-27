/*
** EPITECH PROJECT, 2018
** CPP_babel_2018
** File description:
** Main.cpp
*/

#include "ServerCrypto.h"

ServerCrypto::ServerCrypto()
{
	_localKeyPairRSA = NULL;

	initialize();
}

ServerCrypto::~ServerCrypto()
{
	EVP_CIPHER_CTX_free(_decryptContextRSA);
}

int ServerCrypto::initialize()
{
	_decryptContextRSA = EVP_CIPHER_CTX_new();

	// Check if any of the contexts initializations failed
	if (_decryptContextRSA == NULL)
		return FAILURE;

	return generateRsaKeypair();
}

int ServerCrypto::generateRsaKeypair()
{
	EVP_PKEY_CTX *context = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);

	if (EVP_PKEY_keygen_init(context) <= 0)
		return FAILURE;

	if (EVP_PKEY_CTX_set_rsa_keygen_bits(context, RSA_KEYLEN) <= 0)
		return FAILURE;

	if (EVP_PKEY_keygen(context, &_localKeyPairRSA) <= 0)
		return FAILURE;

	EVP_PKEY_CTX_free(context);

	return SUCCESS;
}

int ServerCrypto::decryptRSA(unsigned char *encryptedMessage,
	size_t encryptedMessageLength, unsigned char *encryptedKey,
	size_t encryptedKeyLength, unsigned char *iv, size_t ivLength,
	unsigned char **decryptedMessage
)
{
	// Allocate memory for everything
	size_t decryptedMessageLength = 0;
	size_t blockLength = 0;

	*decryptedMessage = (unsigned char *)malloc(
		encryptedMessageLength + ivLength);
	if (*decryptedMessage == NULL)
		return FAILURE;

	EVP_PKEY *key = _localKeyPairRSA;

	// Decrypt it!
	if (!EVP_OpenInit(_decryptContextRSA, EVP_aes_256_cbc(), encryptedKey,
		encryptedKeyLength, iv, key))
		return FAILURE;

	if (!EVP_OpenUpdate(_decryptContextRSA,
		(unsigned char *)*decryptedMessage + decryptedMessageLength,
		(int *)&blockLength, encryptedMessage,
		(int)encryptedMessageLength))
		return FAILURE;
	decryptedMessageLength += blockLength;

	if (!EVP_OpenFinal(_decryptContextRSA,
		(unsigned char *)*decryptedMessage + decryptedMessageLength,
		(int *)&blockLength))
		return FAILURE;
	decryptedMessageLength += blockLength;

	return (int)decryptedMessageLength;
}

std::string ServerCrypto::getLocalPublicKey()
{

	BIO *bio = BIO_new(BIO_s_mem());

	PEM_write_bio_PUBKEY(bio, _localKeyPairRSA);
	int bioLength = BIO_pending(bio);
	std::string key = std::string(bioLength, '\0');
	BIO_read(bio, key.data(), bioLength);
	key = key.substr(0, key.size() - 1);

	BIO_free_all(bio);

	return key;
}

std::string ServerCrypto::getLocalPrivateKey()
{
	BIO *bio = BIO_new(BIO_s_mem());

	PEM_write_bio_PrivateKey(bio, _localKeyPairRSA, NULL, NULL, 0, 0, NULL);
	int bioLength = BIO_pending(bio);
	std::string key = std::string(bioLength, '\0');
	BIO_read(bio, key.data(), bioLength);
	key = key.substr(0, key.size() - 1);

	BIO_free_all(bio);

	return key;
}

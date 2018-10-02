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
	RSA_free(_localKeyPairRSA);
}

void ServerCrypto::initialize()
{
	generateRsaKeyPair();
}

void ServerCrypto::generateRsaKeyPair()
{
	_localKeyPairRSA = RSA_generate_key(RSA_KEYLEN, RSA_F4, NULL, NULL);

	if (RSA_check_key(_localKeyPairRSA) != 1)
		throw CryptoException("Cannot Generate RSA KeyPair");
}

BabelString ServerCrypto::decryptRSA(const BabelString &encryptedMessage)
{
	char decryptedMessageBuffer[4098];
	int decryptedMessageLength = 0;

	decryptedMessageLength = RSA_private_decrypt(encryptedMessage.getSize(), (unsigned char *)encryptedMessage.getData(), (unsigned char *)decryptedMessageBuffer, _localKeyPairRSA, RSA_PADDING);

	if (decryptedMessageLength <= 0)
		throw CryptoException("RSA Encryption Failed");

	return BabelString(decryptedMessageBuffer, decryptedMessageLength);
}

BabelString ServerCrypto::getLocalPublicKey()
{
	BIO *bio = BIO_new(BIO_s_mem());

	PEM_write_bio_RSAPublicKey(bio, _localKeyPairRSA);
	int bioLength = BIO_pending(bio);
	char keyBuffer[bioLength];

	BIO_read(bio, keyBuffer, bioLength);
	BIO_free_all(bio);

	return BabelString(keyBuffer, bioLength - 1);
}

BabelString ServerCrypto::getLocalPrivateKey()
{
	BIO *bio = BIO_new(BIO_s_mem());

	PEM_write_bio_RSAPrivateKey(bio, _localKeyPairRSA, NULL, NULL, 0, 0, NULL);
	int bioLength = BIO_pending(bio);
	char keyBuffer[bioLength];

	BIO_read(bio, keyBuffer, bioLength);
	BIO_free_all(bio);

	return BabelString(keyBuffer, bioLength - 1);
}

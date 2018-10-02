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

	if (_encryptContextAES == NULL || _decryptContextAES == NULL)
		throw CryptoException(
			"Cannot Create AES Encryption & Decryption Context");

	EVP_CIPHER_CTX_init(_encryptContextAES);
	EVP_CIPHER_CTX_init(_decryptContextAES);

	EVP_CipherInit_ex(_encryptContextAES, EVP_aes_256_cbc(), NULL, NULL,
		NULL, 1);

	_aesKey = BabelString('A',
		EVP_CIPHER_CTX_key_length(_encryptContextAES));
	_aesIv = BabelString('A', EVP_CIPHER_CTX_iv_length(_encryptContextAES));
}

BabelString CommonCrypto::encryptAES(const BabelString &message)
{
	size_t blockLength = 0;
	size_t encryptedMessageLength = 0;

	char encryptedMessageBuffer[message.getSize() + AES_BLOCK_SIZE];

	if (!EVP_EncryptInit_ex(_encryptContextAES, EVP_aes_256_cbc(), NULL,
		(unsigned char *)_aesKey.getData(),
		(unsigned char *)_aesIv.getData()))
		throw CryptoException("Cannot Initialize AES Encryption");

	if (!EVP_EncryptUpdate(_encryptContextAES,
		(unsigned char *)encryptedMessageBuffer, (int *)&blockLength,
		(unsigned char *)message.getData(), message.getSize()))
		throw CryptoException("Cannot Update AES Encryption");

	encryptedMessageLength += blockLength;

	if (!EVP_EncryptFinal_ex(_encryptContextAES,
		(unsigned char *)encryptedMessageBuffer +
			encryptedMessageLength, (int *)&blockLength))
		throw CryptoException("Cannot Finalize AES Encryption");

	encryptedMessageLength += blockLength;

	return BabelString(encryptedMessageBuffer, encryptedMessageLength);
}

BabelString CommonCrypto::decryptAES(const BabelString &encryptedMessage)
{
	size_t blockLength = 0;
	size_t decryptedMessageLength = 0;

	char decryptedMessageBuffer[encryptedMessage.getSize()];

	if (!EVP_DecryptInit_ex(_decryptContextAES, EVP_aes_256_cbc(), NULL,
		(unsigned char *)_aesKey.getData(),
		(unsigned char *)_aesIv.getData()))
		throw CryptoException("Cannot Initialize AES Decryption");

	if (!EVP_DecryptUpdate(_decryptContextAES,
		(unsigned char *)decryptedMessageBuffer, (int *)&blockLength,
		(unsigned char *)encryptedMessage.getData(),
		encryptedMessage.getSize()))
		throw CryptoException("Cannot Update AES Decryption");

	decryptedMessageLength += blockLength;

	if (!EVP_DecryptFinal_ex(_decryptContextAES,
		(unsigned char *)decryptedMessageBuffer +
			decryptedMessageLength, (int *)&blockLength))
		throw CryptoException("Cannot Finalize AES Decryption");

	decryptedMessageLength += blockLength;

	return BabelString(decryptedMessageBuffer, decryptedMessageLength);
}

BabelString CommonCrypto::getAESKey()
{
	return _aesKey;
}

void CommonCrypto::setAESKey(const BabelString &aesKey)
{
	if (_aesKey.getSize() != aesKey.getSize())
		throw CryptoException("Cannot Set New AES Key : Wrong Size");

	_aesKey = aesKey;
}

BabelString CommonCrypto::getAESIv()
{
	return _aesIv;
}

void CommonCrypto::setAESIv(const BabelString &aesIv)
{
	if (_aesIv.getSize() != aesIv.getSize())
		throw CryptoException("Cannot Set New AES Iv : Wrong Size");

	_aesIv = aesIv;
}

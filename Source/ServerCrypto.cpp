#include "ServerCrypto.h"

ServerCrypto::ServerCrypto() {
  _localKeyPairRSA = NULL;

  init();
}

ServerCrypto::~ServerCrypto() {
  EVP_CIPHER_CTX_free(_encryptContextAES);

  EVP_CIPHER_CTX_free(_decryptContextRSA);
  EVP_CIPHER_CTX_free(_decryptContextAES);

  free(_aesKey);
  free(_aesIv);
}

int ServerCrypto::init() {
  _decryptContextRSA = EVP_CIPHER_CTX_new();

  _encryptContextAES = EVP_CIPHER_CTX_new();
  _decryptContextAES = EVP_CIPHER_CTX_new();

  // Check if any of the contexts initializations failed
  if(_encryptContextAES == NULL || _decryptContextRSA == NULL || _decryptContextAES == NULL) {
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
  generateRsaKeypair(&_localKeyPairRSA);

  return SUCCESS;
}

int ServerCrypto::generateRsaKeypair(EVP_PKEY **keypair) {
  EVP_PKEY_CTX *context = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);

  if(EVP_PKEY_keygen_init(context) <= 0) {
    return FAILURE;
  }

  if(EVP_PKEY_CTX_set_rsa_keygen_bits(context, RSA_KEYLEN) <= 0) {
    return FAILURE;
  }

  if(EVP_PKEY_keygen(context, keypair) <= 0) {
    return FAILURE;
  }

  EVP_PKEY_CTX_free(context);

  return SUCCESS;
}

int ServerCrypto::decryptRSA(unsigned char *encryptedMessage, size_t encryptedMessageLength,
                             unsigned char *encryptedKey,
                             size_t encryptedKeyLength, unsigned char *iv, size_t ivLength,
                             unsigned char **decryptedMessage) {

  // Allocate memory for everything
  size_t decryptedMessageLength = 0;
  size_t blockLength = 0;

  *decryptedMessage = (unsigned char*)malloc(encryptedMessageLength + ivLength);
  if(*decryptedMessage == NULL) {
    return FAILURE;
  }

  EVP_PKEY *key = _localKeyPairRSA;

  // Decrypt it!
  if(!EVP_OpenInit(_decryptContextRSA, EVP_aes_256_cbc(), encryptedKey, encryptedKeyLength, iv, key)) {
    return FAILURE;
  }

  if(!EVP_OpenUpdate(_decryptContextRSA, (unsigned char*)*decryptedMessage + decryptedMessageLength, (int*)&blockLength, encryptedMessage, (int)encryptedMessageLength)) {
    return FAILURE;
  }
  decryptedMessageLength += blockLength;

  if(!EVP_OpenFinal(_decryptContextRSA, (unsigned char*)*decryptedMessage + decryptedMessageLength, (int*)&blockLength)) {
    return FAILURE;
  }
  decryptedMessageLength += blockLength;

  return (int)decryptedMessageLength;
}

int ServerCrypto::encryptAES(const unsigned char *message, size_t messageLength, unsigned char **encryptedMessage) {
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

int ServerCrypto::decryptAES(unsigned char *encryptedMessage, size_t encryptedMessageLength,
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

std::string ServerCrypto::getLocalPublicKey() {

  BIO *bio = BIO_new(BIO_s_mem());

  PEM_write_bio_PUBKEY(bio, _localKeyPairRSA);
  int bioLength = BIO_pending(bio);
  std::string key = std::string(bioLength, '\0');
  BIO_read(bio, key.data(), bioLength);
  key = key.substr(0, key.size() - 1);

  BIO_free_all(bio);

  return key;
}

std::string ServerCrypto::getLocalPrivateKey() {
  BIO *bio = BIO_new(BIO_s_mem());

  PEM_write_bio_PrivateKey(bio, _localKeyPairRSA, NULL, NULL, 0, 0, NULL);
  int bioLength = BIO_pending(bio);
  std::string key = std::string(bioLength, '\0');
  BIO_read(bio, key.data(), bioLength);
  key = key.substr(0, key.size() - 1);

  BIO_free_all(bio);

  return key;
}

int ServerCrypto::getAesKey(unsigned char **aesKey) {
  *aesKey = this->_aesKey;
  return aesKeyLength;
}

int ServerCrypto::setAesKey(unsigned char *aesKey, size_t aesKeyLengthgth) {
  // Ensure the new key is the proper size
  if(aesKeyLengthgth != aesKeyLength) {
    return FAILURE;
  }

  std::memcpy(this->_aesKey, aesKey, aesKeyLength);

  return SUCCESS;
}

int ServerCrypto::getAesIv(unsigned char **aesIv) {
  *aesIv = this->_aesIv;
  return aesIvLength;
}

int ServerCrypto::setAesIv(unsigned char *aesIv, size_t aesIvLengthgth) {
  // Ensure the new IV is the proper size
  if(aesIvLengthgth != aesIvLength) {
    return FAILURE;
  }

  std::memcpy(this->_aesIv, aesIv, aesIvLength);

  return SUCCESS;
}

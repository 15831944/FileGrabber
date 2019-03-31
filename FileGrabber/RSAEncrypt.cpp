#include "stdafx.h"
#include "RSAEncrypt.h"
#include <openssl/bn.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/bio.h>
#include <openssl/x509.h>
using namespace std;


RSAEncrypt::RSAEncrypt()
{
}

void RSAEncrypt::GenerateRSAKeyFile(std::string privateKeyPath, std::string publicKeyPath)
{
	BIO* publicKeyFile = BIO_new_file(publicKeyPath.c_str(), "w");
	BIO* privateKeyFile = BIO_new_file(privateKeyPath.c_str(), "w");
	RSA* rsa = RSA_new();
	BIGNUM* bignum = BN_new();
	BN_set_word(bignum, RSA_F4);
	RSA_generate_key_ex(rsa, 2048, bignum, NULL);
	EVP_PKEY* pkey = EVP_PKEY_new();
	EVP_PKEY_set1_RSA(pkey, rsa);
	PEM_write_bio_PUBKEY(publicKeyFile, pkey);
	PEM_write_bio_RSAPrivateKey(privateKeyFile, rsa, NULL, NULL, 0, NULL, NULL);
	RSA_free(rsa);
	BIO_free(publicKeyFile);
	BIO_free(privateKeyFile);
	BN_free(bignum);
	EVP_PKEY_free(pkey);
}


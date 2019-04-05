#include "stdafx.h"
#include "AESKey.h"
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/bio.h>
#include <openssl/applink.c>
#include <random>
#include "Convert.h"
#include <exception>
#include <stdexcept>
#include <cstdio>
using namespace std;

AESKey::AESKey(const char* keyPath, const char* rsaPrivateFilePath)
{
	ReadAESKey(keyPath, rsaPrivateFilePath);
}

AESKey::AESKey()
{
	GenerateAESKey();
}

const unsigned char* AESKey::GetKey() const
{
	return key;
}

const unsigned char* AESKey::GetIV() const
{
	return iv;
}

void AESKey::WriteAESKey(const char* path, const char* rsaPublicFilePath) const
{
	FILE *output, *rsaPubKeyFile;
	fopen_s(&output, path, "wb");
	if (output == NULL) {
		throw runtime_error("FileGrabber IO Error: Cannot create AES key file.");
		return;
	}
	fopen_s(&rsaPubKeyFile, rsaPublicFilePath, "rb");
	if (rsaPubKeyFile == NULL) {
		throw runtime_error("FileGrabber IO Error: Invalid path to the RSA public key file.");
		return;
	}
	RSA* rsa = PEM_read_RSAPublicKey(rsaPubKeyFile, NULL, NULL, NULL);
	if (rsa == NULL) {
		throw runtime_error("FileGrabber Encryption Error: Invalid RSA public key file.");
		return;
	}
	unsigned char* data = new unsigned char[512];
	unsigned char* enc = new unsigned char[512];
	memset(enc, 0, 512 * sizeof(unsigned char));
	for (int i = 0; i < 256; ++i) {
		data[i] = key[i];
	}
	for (int i = 0; i < 256; ++i) {
		data[i + 256] = iv[i];
	}
	Convert convert;
	for (int i = 0; i < 512; ++i) {
		OutputDebugString(convert.toString(data[i]).c_str());
	}
	OutputDebugString(L"\n");
	int res = RSA_public_encrypt(512, data, enc, rsa, RSA_NO_PADDING);
	delete[] data;
	data = nullptr;
	if (res == -1) {
		delete[] enc;
		throw runtime_error("FileGrabber Encryption Error: RSA Encryption Failed");
		return;
	}
	fwrite(enc, sizeof(unsigned char), 512, output);
	delete[] enc;
	fclose(output);
	fclose(rsaPubKeyFile);
}

void AESKey::GenerateRSAEncryptionFile(const char* rsaPublicFilePath, const char* rsaPrivateFilePath)
{
	BIO* publicKeyFile = BIO_new_file(rsaPublicFilePath , "w");
	BIO* privateKeyFile = BIO_new_file(rsaPrivateFilePath, "w");
	if (publicKeyFile == NULL || privateKeyFile == NULL) {
		throw runtime_error("FileGrabber IO Error: Invalid RSA file path.");
		return;
	}
	RSA* rsa = RSA_new();
	BIGNUM* bignum = BN_new();
	BN_set_word(bignum, RSA_F4);
	RSA_generate_key_ex(rsa, 4096, bignum, NULL);
	PEM_write_bio_RSAPublicKey(publicKeyFile, rsa);
	PEM_write_bio_RSAPrivateKey(privateKeyFile, rsa, NULL, NULL, 0, NULL, NULL);
	RSA_free(rsa);
	BIO_free(publicKeyFile);
	BIO_free(privateKeyFile);
	BN_free(bignum);
}

void AESKey::ReadAESKey(const char* keyPath, const char* rsaPrivateFilePath)
{
	FILE *rsaPriKeyFile, *keyFile;
	fopen_s(&rsaPriKeyFile, rsaPrivateFilePath, "rb");
	if (rsaPriKeyFile == NULL) {
		throw runtime_error("FileGrabber IO Error: Invalid path to the RSA private key file.");
		return;
	}
	fopen_s(&keyFile, keyPath, "rb");
	if (keyFile == NULL) {
		throw runtime_error("FileGrabber IO Error: Invalid path to the AES key file.");
		return;
	}
	RSA* rsa = PEM_read_RSAPrivateKey(rsaPriKeyFile, NULL, NULL, NULL);
	if (rsa == NULL) {
		throw runtime_error("FileGrabber Encryption Error: Invalid RSA private key file.");
		return;
	}
	unsigned char* enc = new unsigned char[1024];
	unsigned char* data = new unsigned char[512];
	fread_s(enc, 512 * sizeof(unsigned char), sizeof(unsigned char), 512, keyFile);
	int res = RSA_private_decrypt(512, enc, data, rsa, RSA_NO_PADDING);
	delete[] enc;
	enc = nullptr;
	if (res == -1) {
		delete[] enc;
		throw runtime_error("FileGrabber Decryption Error: RSA Decryption Failed");
		return;
	}
	for (int i = 0; i < 256; ++i) {
		key[i] = data[i];
	}
	for (int i = 0; i < 256; ++i) {
		iv[i] = data[i + 256];
	}
}

void AESKey::GenerateAESKey()
{
	default_random_engine engine;
	engine.seed(time(NULL));
	uniform_int_distribution<unsigned int> gen(0, 255);
	for (int i = 0; i < 256; ++i) {
		key[i] = (unsigned char)gen(engine);
	}
	for (int i = 0; i < 256; ++i) {
		iv[i] = (unsigned char)gen(engine);
	}
}

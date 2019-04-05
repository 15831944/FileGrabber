#include "stdafx.h"
#include "RSAEncrypt.h"
#include <openssl/bn.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/bio.h>
#include <openssl/x509.h>
#include <openssl/applink.c>
#include <exception>
#include <filesystem>
#include "Convert.h"
using namespace std;
using namespace std::filesystem;

RSAEncrypt::RSAEncrypt(const std::string& publicKeyPath)
{
	this->publicKeyPath = publicKeyPath;
	FILE* pubKeyReader;
	fopen_s(&pubKeyReader, publicKeyPath.c_str(), "rb");
	if (pubKeyReader == NULL) {
		throw runtime_error("FileGrabber IO Error: Invalid path to the public key.");
		return;
	}
	RSA* rsa = PEM_read_RSAPublicKey(pubKeyReader, NULL, NULL, NULL);
	this->key = rsa;
	if (pubKeyReader != NULL)
		fclose(pubKeyReader);
	int flen = RSA_size(key);
	Convert convert;
	OutputDebugStringW(convert.toString(flen).c_str());
}

RSAEncrypt::~RSAEncrypt()
{
	RSA_free(key);
}

void RSAEncrypt::GenerateRSAKeyFile(std::string privateKeyPath, std::string publicKeyPath) noexcept
{
	BIO* publicKeyFile = BIO_new_file(publicKeyPath.c_str(), "w");
	BIO* privateKeyFile = BIO_new_file(privateKeyPath.c_str(), "w");
	RSA* rsa = RSA_new();
	BIGNUM* bignum = BN_new();
	BN_set_word(bignum, RSA_F4);
	RSA_generate_key_ex(rsa, 2048, bignum, NULL);
	PEM_write_bio_RSAPublicKey(publicKeyFile, rsa);
	PEM_write_bio_RSAPrivateKey(privateKeyFile, rsa, NULL, NULL, 0, NULL, NULL);
	RSA_free(rsa);
	BIO_free(publicKeyFile);
	BIO_free(privateKeyFile);
	BN_free(bignum);
}

void RSAEncrypt::Encrypt(std::string filePath)
{

}

void RSAEncrypt::Encrypt(std::string filePath, std::string outputFilePath)
{
	FILE *originFile, *newFile;
	fopen_s(&originFile, filePath.c_str(), "rb");
	fopen_s(&newFile, outputFilePath.c_str(), "wb");
	if (originFile == NULL) {
		throw runtime_error(("FileGrabber IO Error: File \"" + filePath + "\" does not exist."));
		return;
	}
	if (newFile == NULL) {
		throw runtime_error(("FileGrabber IO Error: Cannot create and write the encrypted file."));
		return;
	}
	fseek(originFile, 0, SEEK_END);
	long len = ftell(originFile);
	fseek(originFile, 0, SEEK_SET);
	if (len <= 256) {
		int maxlen = len;
		unsigned char* buffer = new unsigned char[maxlen];
		size_t reallen = fread_s(buffer, sizeof(unsigned char) * maxlen, sizeof(unsigned char), maxlen, originFile);
		unsigned char* encBuffer = new unsigned char[maxlen];
		this->Encrypt(buffer, encBuffer, (int)reallen);
		fwrite(encBuffer, sizeof(unsigned char), reallen, newFile);
		delete[] buffer;
		delete[] encBuffer;
		fclose(originFile);
		fclose(newFile);
	}
	else {
		int maxlen = len;
		int remainlen = maxlen;
		unsigned char *buffer = new unsigned char[200];
		unsigned char *encBuffer = new unsigned char[256];
		while (true) {
			if (remainlen >= 200) {
				fread_s(buffer, 200 * sizeof(unsigned char), sizeof(unsigned char), 200, originFile);
				memset(encBuffer, 0, 256 * sizeof(unsigned char));
				this->Encrypt(buffer, encBuffer, 200);
				fwrite(encBuffer, sizeof(unsigned char), 256, newFile);
				remainlen -= 200;
			}
			else {
				int maxlen = remainlen;
				unsigned char* buffer = new unsigned char[maxlen];
				size_t reallen = fread_s(buffer, sizeof(unsigned char) * maxlen, sizeof(unsigned char), maxlen, originFile);
				unsigned char* encBuffer = new unsigned char[256];
				memset(encBuffer, 0, 256 * sizeof(unsigned char));
				this->Encrypt(buffer, encBuffer, (int)reallen);
				fwrite(encBuffer, sizeof(unsigned char), reallen, newFile);
				delete[] buffer;
				delete[] encBuffer;
				fclose(originFile);
				fclose(newFile);
				break;
			}
		}
		delete[] buffer;
		delete[] encBuffer;
	}
}

void RSAEncrypt::Encrypt(const unsigned char* source, unsigned char* buffer, int byteCount)
{
	RSA* rsa = key;
	int res = RSA_public_encrypt(byteCount, source, buffer, rsa, RSA_PKCS1_OAEP_PADDING);
	if (res == -1) {
		throw runtime_error("FileGrabber Encryption Error: RSA encryption failed.");
	}
}



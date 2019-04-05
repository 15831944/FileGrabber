#include "stdafx.h"
#include "AESEncrypt.h"
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <exception>
#include <stdexcept>
using namespace std;

void AESEncrypt::Encrypt(const TCHAR* path, const TCHAR* outputPath)
{
	FILE *origin, *encryption;
	_tfopen_s(&origin, path, TEXT("rb"));
	if (origin == NULL) {
		throw runtime_error("FileGrabber IO Error: Invalid path to the origin file.");
		return;
	}
	_tfopen_s(&encryption, outputPath, TEXT("wb"));
	if (encryption == NULL) {
		fclose(origin);
		throw runtime_error("FileGrabber IO Error: Cannot open encryption file to write.");
		return;
	}
	fseek(origin, 0, SEEK_END);
	int len = ftell(origin);
	fseek(origin, 0, SEEK_SET);
	EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
	EVP_CIPHER_CTX_set_padding(ctx, EVP_PADDING_PKCS7);
	EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key.GetKey(), key.GetIV());
	int outl;
	unsigned char buffer[256], enc[256];
	while (true) {
		if (len > 256) {
			fread_s(buffer, 256 * sizeof(unsigned char), sizeof(unsigned char), 256, origin);
			EVP_EncryptUpdate(ctx, enc, &outl, buffer, 256);
			fwrite(enc, sizeof(unsigned char), 256, encryption);
			len -= 256;
		}
		else {
			fread_s(buffer, len * sizeof(unsigned char), sizeof(unsigned char), len, origin);
			EVP_EncryptUpdate(ctx, enc, &outl, buffer, len);
			fwrite(enc, sizeof(unsigned char), outl, encryption);
			EVP_EncryptFinal_ex(ctx, enc, &outl);
			fwrite(enc, sizeof(unsigned char), outl, encryption);
			break;
		}
	}
	fclose(origin);
	fclose(encryption);
}

void AESEncrypt::Decrypt(const TCHAR* path, const TCHAR* outputPath)
{
	FILE* origin, * decryption;
	_tfopen_s(&origin, path, TEXT("rb"));
	if (origin == NULL) {
		throw runtime_error("FileGrabber IO Error: Invalid path to the origin file.");
		return;
	}
	_tfopen_s(&decryption, outputPath, TEXT("wb"));
	if (decryption == NULL) {
		fclose(origin);
		throw runtime_error("FileGrabber IO Error: Cannot open encryption file to write.");
		return;
	}
	fseek(origin, 0, SEEK_END);
	int len = ftell(origin);
	fseek(origin, 0, SEEK_SET);
	EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
	EVP_CIPHER_CTX_set_padding(ctx, EVP_PADDING_PKCS7);
	EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key.GetKey(), key.GetIV());
	int outl;
	unsigned char buffer[256], enc[512];
	while (true) {
		if (len > 256) {
			fread_s(buffer, 256 * sizeof(unsigned char), sizeof(unsigned char), 256, origin);
			EVP_DecryptUpdate(ctx, enc, &outl, buffer, 256);
			fwrite(enc, sizeof(unsigned char), outl, decryption);
			len -= 256;
		}
		else {
			fread_s(buffer, 256 * sizeof(unsigned char), sizeof(unsigned char), len, origin);
			EVP_DecryptUpdate(ctx, enc, &outl, buffer, len);
			fwrite(enc, sizeof(unsigned char), outl, decryption);
			break;
		}
	}
	EVP_DecryptFinal_ex(ctx, enc, &outl);
	fwrite(enc, sizeof(unsigned char), outl, decryption);
	fclose(origin);
	fclose(decryption);
}

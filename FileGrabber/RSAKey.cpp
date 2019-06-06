#include "stdafx.h"
#include "RSAKey.h"
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/evp.h>
#include "Convert.h"
using namespace std;

RSAKey::RSAKey()
{
	pubkey = prikey = key = nullptr;
	type = KeyType::NONE;
}

RSAKey::~RSAKey()
{
	if (key != nullptr)
		RSA_free(key);
}

void RSAKey::generate()
{
	key = RSA_new();
	BIGNUM* bn = BN_new();
	BN_set_word(bn, RSA_F4);
	RSA_generate_key_ex(key, 2048, bn, NULL);
	type = KeyType::ALL;
}

void RSAKey::setPublicKey(const std::string& path)
{
	if (type != KeyType::NONE && type != KeyType::PRIVATE) {
		return;
	}

	FILE* file;
	fopen_s(&file, path.c_str(), "r");
	PEM_read_RSAPublicKey(file, &pubkey, NULL, NULL);
	fclose(file);

	if (type == KeyType::NONE) {
		type = KeyType::PUBLIC;
	}
	else {
		type = KeyType::ALL;
	}
}

void RSAKey::setPublicKey(const std::wstring& path)
{
	if (type != KeyType::NONE && type != KeyType::PRIVATE) {
		return;
	}

	FILE* file;
	_wfopen_s(&file, path.c_str(), L"r");
	PEM_read_RSAPublicKey(file, &pubkey, NULL, NULL);
	fclose(file);

	if (type == KeyType::NONE) {
		type = KeyType::PUBLIC;
	}
	else {
		type = KeyType::ALL;
	}
}

void RSAKey::setPrivateKey(const std::string& path)
{
	if (type != KeyType::NONE && type != KeyType::PUBLIC) {
		return;
	}

	FILE* file;
	fopen_s(&file, path.c_str(), "r");
	PEM_read_RSAPrivateKey(file, &pubkey, NULL, NULL);
	fclose(file);

	if (type == KeyType::NONE) {
		type = KeyType::PRIVATE;
	}
	else {
		type = KeyType::ALL;
	}
}

void RSAKey::setPrivateKey(const std::wstring& path)
{
	if (type != KeyType::NONE && type != KeyType::PUBLIC) {
		return;
	}

	FILE* file;
	_wfopen_s(&file, path.c_str(), L"r");
	PEM_read_RSAPrivateKey(file, &pubkey, NULL, NULL);
	fclose(file);

	if (type == KeyType::NONE) {
		type = KeyType::PRIVATE;
	}
	else {
		type = KeyType::ALL;
	}
}

RSA* RSAKey::getRSAKey()
{
	return key;
}

void RSAKey::writePublicKey(const std::string& path)
{
	FILE* file;
	fopen_s(&file, path.c_str(), "w");
	switch (type) {
	case KeyType::ALL:
	case KeyType::PUBLIC:
		PEM_write_RSAPublicKey(file, pubkey);
		break;
	case KeyType::GENERATED_INSTANCE:
		PEM_write_RSAPublicKey(file, key);
		break;
	}
	fclose(file);
}

void RSAKey::writePublicKey(const std::wstring& path)
{
	FILE* file;
	_wfopen_s(&file, path.c_str(), L"w");
	switch (type) {
	case KeyType::ALL:
	case KeyType::PUBLIC:
		PEM_write_RSAPublicKey(file, pubkey);
		break;
	case KeyType::GENERATED_INSTANCE:
		PEM_write_RSAPublicKey(file, key);
		break;
	}
	fclose(file);
}

void RSAKey::writePrivateKey(const std::string& path)
{
	FILE* file;
	fopen_s(&file, path.c_str(), "w");
	switch (type) {
	case KeyType::ALL:
	case KeyType::PRIVATE:
		PEM_write_RSAPrivateKey(file, prikey, NULL, NULL, 0, NULL, NULL);
		break;
	case KeyType::GENERATED_INSTANCE:
		PEM_write_RSAPrivateKey(file, key, NULL, NULL, 0, NULL, NULL);
		break;
	}
	fclose(file);
}

void RSAKey::writePrivateKey(const std::wstring& path)
{
	FILE* file;
	_wfopen_s(&file, path.c_str(), L"w");
	switch (type) {
	case KeyType::ALL:
	case KeyType::PRIVATE:
		PEM_write_RSAPrivateKey(file, prikey, NULL, NULL, 0, NULL, NULL);
		break;
	case KeyType::GENERATED_INSTANCE:
		PEM_write_RSAPrivateKey(file, key, NULL, NULL, 0, NULL, NULL);
		break;
	}
	fclose(file);
}

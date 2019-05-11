#pragma once
#include "AESKey.h"
class AESEncrypt
{
public:
	AESEncrypt(AESKey key) : key(key) {}
	void Encrypt(const TCHAR* path, const TCHAR* outputPath);
	void Decrypt(const TCHAR* path, const TCHAR* outputPath);
	//std::string EncryptFileName(const TCHAR* filename);
	//std::string DecryptFileName(const TCHAR* filename);
private:
	AESKey key;
};
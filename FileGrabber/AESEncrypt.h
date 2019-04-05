#pragma once
#include "AESKey.h"
class AESEncrypt
{
public:
	AESEncrypt(AESKey key) : key(key) {}
	void Encrypt(const TCHAR* path, const TCHAR* outputPath);
	void Encrypt(const TCHAR* path);
	void Decrypt(const TCHAR* path, const TCHAR* outputPath);
private:
	AESKey key;
};
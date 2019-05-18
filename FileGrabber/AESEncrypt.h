#pragma once
#include "AESKey.h"
#include <string>
class AESEncrypt
{
public:
	AESEncrypt(AESKey key) : key(key) {}
	int encrypt(const unsigned char* origin, int byteCount, unsigned char* dst);
	int decrypt(const unsigned char* origin, int byteCount, unsigned char* dst);
	void encryptFile(const std::wstring& path, const std::wstring& outputPath);
	void decryptFile(const std::wstring& path, const std::wstring& outputPath);
private:
	AESKey key;
};
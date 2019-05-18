#pragma once
#include <string>

class MD5Encrypt
{
public:
	MD5Encrypt();
	struct MD5Hash {
		MD5Hash();
		unsigned char _data[16];
		operator unsigned char* ();
		std::wstring toString();
		std::string toANSIString();
	};
	MD5Hash encryptString(const std::wstring& val);
	MD5Hash encryptFile(const std::wstring& path);
	MD5Hash encrypt(const unsigned char* data, size_t byteCount);
};
#pragma once
#include <string>

class MD5Encrypt
{
public:
	MD5Encrypt();
	struct MD5Hash {
		unsigned char _data[32];
	};
	std::wstring EncryptString(const std::wstring& val);
	MD5Hash HashString(const std::wstring& val);
};


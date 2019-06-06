#pragma once
#include <string>
class Base64Encoder
{
public:
	Base64Encoder();
	Base64Encoder(const unsigned char* data, int length);
	Base64Encoder(const std::string& str);
	Base64Encoder(const std::wstring& str);
	~Base64Encoder();
	std::wstring toString() const;
	std::string toANSIString() const;
	void release();
protected:
	unsigned char* _data;
};
#pragma once
#include <string>
class Base64Decoder
{
public:
	Base64Decoder();
	Base64Decoder(const std::string& val);
	Base64Decoder(const std::wstring& val);
	~Base64Decoder();
	unsigned char* getData() const;
	std::wstring toString() const;
	std::string toANSIString() const;
	void release();
	int getDataLength() const;
protected:
	unsigned char* _data;
	int length;
};


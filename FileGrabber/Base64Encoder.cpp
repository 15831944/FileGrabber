#include "stdafx.h"
#include "Base64Encoder.h"
#include "Convert.h"
#include <openssl/evp.h>
using namespace std;

Base64Encoder::Base64Encoder()
{
	this->_data = nullptr;
}

Base64Encoder::Base64Encoder(const unsigned char* data, int length)
{
	this->_data = new unsigned char[(static_cast<size_t>(length) + 1) * 3 + 1];
	EVP_EncodeBlock(this->_data, data, length);
}

Base64Encoder::Base64Encoder(const std::string& str)
{
	this->_data = new unsigned char[(str.length() + 2) * 3 + 1];
	EVP_EncodeBlock(this->_data, reinterpret_cast<const unsigned char*>(str.c_str()), static_cast<int>(str.length()));
}

Base64Encoder::Base64Encoder(const std::wstring& str)
{
	this->_data = new unsigned char[(str.length() * 2 + 2) * 3 + 1];
	EVP_EncodeBlock(this->_data, reinterpret_cast<const unsigned char*>(str.c_str()), static_cast<int>(str.length() * 2));
}

Base64Encoder::~Base64Encoder()
{
	if (this->_data != nullptr)
		delete[] this->_data;
}

wstring Base64Encoder::toString() const
{
	if (this->_data != nullptr) {
		return Convert().toString(reinterpret_cast<const char*>(this->_data));
	}
	else {
		return wstring();
	}
}

string Base64Encoder::toANSIString() const
{
	if (this->_data != nullptr) {
		return reinterpret_cast<const char*>(this->_data);
	}
	else {
		return string();
	}
}

void Base64Encoder::release()
{
	if (this->_data != nullptr) {
		delete[] this->_data;
		this->_data = nullptr;
	}
}

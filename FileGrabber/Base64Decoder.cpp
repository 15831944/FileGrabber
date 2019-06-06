#include "stdafx.h"
#include "Base64Decoder.h"
#include "Convert.h"
#include <openssl/evp.h>
using namespace std;

Base64Decoder::Base64Decoder()
{
	this->length = 0;
	this->_data = nullptr;
}

Base64Decoder::Base64Decoder(const std::string& val)
{
	this->length = static_cast<int>(val.length() * 3 / 4);
	this->_data = new unsigned char[length];
	EVP_DecodeBlock(this->_data, reinterpret_cast<const unsigned char*>(val.c_str()), static_cast<int>(val.length()));
}

Base64Decoder::Base64Decoder(const std::wstring& val)
{
	this->length = static_cast<int>(val.length() * 3 / 4);
	this->_data = new unsigned char[length];
	EVP_DecodeBlock(this->_data, reinterpret_cast<const unsigned char*>(Convert().toANSIString(val).c_str()), static_cast<int>(val.length()));
}

Base64Decoder::~Base64Decoder()
{
	if (this->_data != nullptr)
		delete[] this->_data;
}

unsigned char* Base64Decoder::getData() const
{
	return this->_data;
}

wstring Base64Decoder::toString() const
{
	if (this->_data != nullptr) {
		wchar_t* pstr = new wchar_t[static_cast<size_t>(length / 2) + 1];
		const unsigned char* dptr = this->_data;
		wchar_t* tptr = pstr;
		for (int i = 0; i < length / 2; ++i) {
			*tptr = (static_cast<wchar_t>(*(dptr + 1)) << 8) + *dptr;
			dptr += 2;
			++tptr;
		}
		*tptr = L'\0';
		wstring temp = pstr;
		delete[] pstr;
		return temp;
	}
	else {
		return wstring();
	}
}

string Base64Decoder::toANSIString() const
{
	if (this->_data != nullptr) {
		char* pstr = new char[static_cast<size_t>(length) + 1];
		const unsigned char* dptr = this->_data;
		char* tptr = pstr;
		for (int i = 0; i < length; ++i) {
			*tptr = *dptr;
			++dptr;
			++tptr;
		}
		*tptr = '\0';
		string temp = pstr;
		delete[] pstr;
		return temp;
	}
	else {
		return string();
	}
}

void Base64Decoder::release()
{
	if (this->_data != nullptr) {
		delete[] this->_data;
		this->_data = nullptr;
	}
}

int Base64Decoder::getDataLength() const
{
	return this->length;
}

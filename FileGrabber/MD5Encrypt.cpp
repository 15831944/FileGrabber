#include "stdafx.h"
#include "MD5Encrypt.h"
#include <sstream>
#include <iomanip>
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <filesystem>
#include "Convert.h"
using namespace std;

MD5Encrypt::MD5Encrypt()
{
}

MD5Encrypt::MD5Hash MD5Encrypt::encryptString(const wstring& val)
{
	MD5Hash hash;
	size_t len = val.length() * 2;
	MD5(reinterpret_cast<const unsigned char*>(val.c_str()), len, hash);
	return hash;
}

MD5Encrypt::MD5Hash MD5Encrypt::encryptFile(const std::wstring& path)
{
	if (!filesystem::exists(path)) {
		throw runtime_error("FileGrabber IO Error: File \'" + Convert().toANSIString(path) + "\' does not exist.");
	}

	MD5_CTX ctx;
	MD5Hash res;
	unsigned char* buffer = new unsigned char[10240];
	size_t len;
	FILE* file;
	errno_t err;
	err = _wfopen_s(&file, path.c_str(), L"r");
	if (file == NULL) {
		throw runtime_error("FileGrabber IO Error: Cannot open file \'" + Convert().toANSIString(path) + "\', error code: " + Convert().toANSIString(err) + ".");
	}

	MD5_Init(&ctx);
	while (len = fread_s(buffer, 10240, sizeof(unsigned char), 10240, file)) {
		MD5_Update(&ctx, buffer, len);
	}
	MD5_Final(res, &ctx);
	return res;
}

MD5Encrypt::MD5Hash MD5Encrypt::encrypt(const unsigned char* data, size_t byteCount)
{
	MD5Hash res;
	MD5(data, byteCount, res);
	return res;
}

MD5Encrypt::MD5Hash::MD5Hash()
{
	for (int i = 0; i < 16; ++i) {
		_data[i] = 0;
	}
}

MD5Encrypt::MD5Hash::operator unsigned char* ()
{
	return _data;
}

wstring MD5Encrypt::MD5Hash::toString()
{
	wstringstream ss;
	for (int i = 0; i < 16; ++i) {
		ss << hex << setw(2) << setfill(L'0') << _data[i];
	}
	wstring res;
	ss >> res;
	return res;
}

string MD5Encrypt::MD5Hash::toANSIString()
{
	stringstream ss;
	for (int i = 0; i < 16; ++i) {
		ss << hex << setw(2) << setfill('0') << _data[i];
	}
	string res;
	ss >> res;
	return res;
}

#include "stdafx.h"
#include "SHAEncrypt.h"
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>
#include <filesystem>
#include "Convert.h"
using namespace std;

SHAEncrypt::SHAEncrypt()
{
}

SHAEncrypt::SHA1Hash::SHA1Hash()
{
	for (int i = 0; i < 20; ++i) {
		_data[i] = 0;
	}
}

SHAEncrypt::SHA1Hash::operator unsigned char* ()
{
	return _data;
}

wstring SHAEncrypt::SHA1Hash::toString()
{
	wstringstream ss;
	for (int i = 0; i < 20; ++i) {
		ss << hex << setw(2) << setfill(L'0') << _data[i];
	}
	wstring res;
	ss >> res;
	return res;
}

string SHAEncrypt::SHA1Hash::toANSIString()
{
	stringstream ss;
	for (int i = 0; i < 20; ++i) {
		ss << hex << setw(2) << setfill('0') << _data[i];
	}
	string res;
	ss >> res;
	return res;
}

SHAEncrypt::SHA256Hash::SHA256Hash()
{
	for (int i = 0; i < 32; ++i) {
		_data[i] = 0;
	}
}

SHAEncrypt::SHA256Hash::operator unsigned char* ()
{
	return _data;
}

wstring SHAEncrypt::SHA256Hash::toString()
{
	wstringstream ss;
	for (int i = 0; i < 32; ++i) {
		ss << hex << setw(2) << setfill(L'0') << _data[i];
	}
	wstring res;
	ss >> res;
	return res;
}

string SHAEncrypt::SHA256Hash::toANSIString()
{
	stringstream ss;
	for (int i = 0; i < 32; ++i) {
		ss << hex << setw(2) << setfill('0') << _data[i];
	}
	string res;
	ss >> res;
	return res;
}

SHAEncrypt::SHA384Hash::SHA384Hash()
{
	for (int i = 0; i < 48; ++i) {
		_data[i] = 0;
	}
}

SHAEncrypt::SHA384Hash::operator unsigned char* ()
{
	return _data;
}

wstring SHAEncrypt::SHA384Hash::toString()
{
	wstringstream ss;
	for (int i = 0; i < 48; ++i) {
		ss << hex << setw(2) << setfill(L'0') << _data[i];
	}
	wstring res;
	ss >> res;
	return res;
}

string SHAEncrypt::SHA384Hash::toANSIString()
{
	stringstream ss;
	for (int i = 0; i < 48; ++i) {
		ss << hex << setw(2) << setfill('0') << _data[i];
	}
	string res;
	ss >> res;
	return res;
}

SHAEncrypt::SHA512Hash::SHA512Hash()
{
	for (int i = 0; i < 64; ++i) {
		_data[i] = 0;
	}
}

SHAEncrypt::SHA512Hash::operator unsigned char* ()
{
	return _data;
}

wstring SHAEncrypt::SHA512Hash::toString()
{
	wstringstream ss;
	for (int i = 0; i < 64; ++i) {
		ss << hex << setw(2) << setfill(L'0') << _data[i];
	}
	wstring res;
	ss >> res;
	return res;
}

string SHAEncrypt::SHA512Hash::toANSIString()
{
	stringstream ss;
	for (int i = 0; i < 64; ++i) {
		ss << hex << setw(2) << setfill('0') << _data[i];
	}
	string res;
	ss >> res;
	return res;
}

SHAEncrypt::SHA1Hash SHAEncrypt::SHA1EncryptString(const std::wstring& val)
{
	SHA1Hash hash;
	size_t len = val.length() * 2;
	SHA1(reinterpret_cast<const unsigned char*>(val.c_str()), len, hash);
	return hash;
}

SHAEncrypt::SHA1Hash SHAEncrypt::SHA1EncryptFile(const std::wstring& path)
{
	if (!filesystem::exists(path)) {
		throw runtime_error("FileGrabber IO Error: File \'" + Convert().toANSIString(path) + "\' does not exist.");
	}

	SHA_CTX ctx;
	SHA1Hash res;
	unsigned char* buffer = new unsigned char[10240];
	size_t len;
	FILE* file;
	errno_t err;
	err = _wfopen_s(&file, path.c_str(), L"r");
	if (file == NULL) {
		throw runtime_error("FileGrabber IO Error: Cannot open file \'" + Convert().toANSIString(path) + "\', error code: " + Convert().toANSIString(err) + ".");
	}

	SHA1_Init(&ctx);
	while (len = fread_s(buffer, 10240, sizeof(unsigned char), 10240, file)) {
		SHA1_Update(&ctx, buffer, len);
	}
	SHA1_Final(res, &ctx);
	return res;
}

SHAEncrypt::SHA1Hash SHAEncrypt::SHA1Encrypt(const unsigned char* data, size_t byteCount)
{
	SHA1Hash hash;
	SHA1(data, byteCount, hash);
	return hash;
}

SHAEncrypt::SHA256Hash SHAEncrypt::SHA256EncryptString(const std::wstring& val)
{
	SHA256Hash hash;
	size_t len = val.length() * 2;
	SHA256(reinterpret_cast<const unsigned char*>(val.c_str()), len, hash);
	return hash;
}

SHAEncrypt::SHA256Hash SHAEncrypt::SHA256EncryptFile(const std::wstring& path)
{
	if (!filesystem::exists(path)) {
		throw runtime_error("FileGrabber IO Error: File \'" + Convert().toANSIString(path) + "\' does not exist.");
	}

	SHA256_CTX ctx;
	SHA256Hash res;
	unsigned char* buffer = new unsigned char[10240];
	size_t len;
	FILE* file;
	errno_t err;
	err = _wfopen_s(&file, path.c_str(), L"r");
	if (file == NULL) {
		throw runtime_error("FileGrabber IO Error: Cannot open file \'" + Convert().toANSIString(path) + "\', error code: " + Convert().toANSIString(err) + ".");
	}

	SHA256_Init(&ctx);
	while (len = fread_s(buffer, 10240, sizeof(unsigned char), 10240, file)) {
		SHA256_Update(&ctx, buffer, len);
	}
	SHA256_Final(res, &ctx);
	return res;
}

SHAEncrypt::SHA256Hash SHAEncrypt::SHA256Encrypt(const unsigned char* data, size_t byteCount)
{
	SHA256Hash hash;
	SHA256(data, byteCount, hash);
	return hash;
}

SHAEncrypt::SHA384Hash SHAEncrypt::SHA384EncryptString(const std::wstring& val)
{
	SHA384Hash hash;
	size_t len = val.length() * 2;
	SHA384(reinterpret_cast<const unsigned char*>(val.c_str()), len, hash);
	return hash;
}

SHAEncrypt::SHA384Hash SHAEncrypt::SHA384EncryptFile(const std::wstring& path)
{
	if (!filesystem::exists(path)) {
		throw runtime_error("FileGrabber IO Error: File \'" + Convert().toANSIString(path) + "\' does not exist.");
	}

	SHA512_CTX ctx;
	SHA384Hash res;
	unsigned char* buffer = new unsigned char[10240];
	size_t len;
	FILE* file;
	errno_t err;
	err = _wfopen_s(&file, path.c_str(), L"r");
	if (file == NULL) {
		throw runtime_error("FileGrabber IO Error: Cannot open file \'" + Convert().toANSIString(path) + "\', error code: " + Convert().toANSIString(err) + ".");
	}

	SHA384_Init(&ctx);
	while (len = fread_s(buffer, 10240, sizeof(unsigned char), 10240, file)) {
		SHA384_Update(&ctx, buffer, len);
	}
	SHA384_Final(res, &ctx);
	return res;
}

SHAEncrypt::SHA384Hash SHAEncrypt::SHA384Encrypt(const unsigned char* data, size_t byteCount)
{
	SHA384Hash hash;
	SHA384(data, byteCount, hash);
	return hash;
}

SHAEncrypt::SHA512Hash SHAEncrypt::SHA512EncryptString(const std::wstring& val)
{
	SHA512Hash hash;
	size_t len = val.length() * 2;
	SHA512(reinterpret_cast<const unsigned char*>(val.c_str()), len, hash);
	return hash;
}

SHAEncrypt::SHA512Hash SHAEncrypt::SHA512EncryptFile(const std::wstring& path)
{
	if (!filesystem::exists(path)) {
		throw runtime_error("FileGrabber IO Error: File \'" + Convert().toANSIString(path) + "\' does not exist.");
	}

	SHA512_CTX ctx;
	SHA512Hash res;
	unsigned char* buffer = new unsigned char[10240];
	size_t len;
	FILE* file;
	errno_t err;
	err = _wfopen_s(&file, path.c_str(), L"r");
	if (file == NULL) {
		throw runtime_error("FileGrabber IO Error: Cannot open file \'" + Convert().toANSIString(path) + "\', error code: " + Convert().toANSIString(err) + ".");
	}

	SHA512_Init(&ctx);
	while (len = fread_s(buffer, 10240, sizeof(unsigned char), 10240, file)) {
		SHA512_Update(&ctx, buffer, len);
	}
	SHA512_Final(res, &ctx);
	return res;
}

SHAEncrypt::SHA512Hash SHAEncrypt::SHA512Encrypt(const unsigned char* data, size_t byteCount)
{
	SHA512Hash hash;
	SHA512(data, byteCount, hash);
	return hash;
}

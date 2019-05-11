#include "stdafx.h"
#include "MD5Encrypt.h"
#include <sstream>
#include <iomanip>
#include <openssl/md5.h>
#include <openssl/sha.h>
using namespace std;

MD5Encrypt::MD5Encrypt()
{
}

wstring MD5Encrypt::EncryptString(const wstring& val)
{
	size_t len = val.length() * 2;
	unsigned char hash[16] = { 0 };
	MD5(reinterpret_cast<const unsigned char*>(val.c_str()), len, hash);
	wstringstream ss;
	for (int i = 0; i < 16; ++i) {
		ss << hex << setw(2) << setfill(L'0') << hash[i];
	}
	wstring res;
	ss >> res;
	return res;
}

#include "stdafx.h"
#include "Base64.h"
#include <openssl/pem.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include "Convert.h"
using namespace std;


Base64::Base64()
{
	this->mode = EncodingMode::BASE64_NO_NEWLINE;
}

Base64::Base64(EncodingMode mode)
{
	this->mode = mode;
}

string Base64::encodeString(const unsigned char* data, int length) const
{
	BIO *b64, *bio;
	BUF_MEM* bptr = NULL;
	int size = 0;

	size_t outlen = (static_cast<size_t>(length) / 3 + 1) * 4 + 1;
	char* res = new char[outlen];

	b64 = BIO_new(BIO_f_base64());
	if (mode == EncodingMode::BASE64_NO_NEWLINE)
		BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);

	bio = BIO_new(BIO_s_mem());
	bio = BIO_push(b64, bio);
	BIO_write(bio, data, length);
	BIO_flush(bio);

	BIO_get_mem_ptr(bio, &bptr);
	memcpy_s(res, outlen, bptr->data, bptr->length);
	res[bptr->length] = 0;

	BIO_free_all(bio);
	string s = res;
	delete[] res;
	return s;
}

string Base64::encodeString(wstring& str) const
{
	return encodeString(reinterpret_cast<const unsigned char*>(str.c_str()), str.length() * 2);
}


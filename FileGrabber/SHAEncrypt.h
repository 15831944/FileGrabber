#pragma once
class SHAEncrypt
{
public:
	SHAEncrypt();
	struct SHA1Hash {
		SHA1Hash();
		unsigned char _data[20];
		operator unsigned char* ();
		std::wstring toString();
		std::string toANSIString();
	};
	struct SHA256Hash {
		SHA256Hash();
		unsigned char _data[32];
		operator unsigned char* ();
		std::wstring toString();
		std::string toANSIString();
	};
	struct SHA384Hash {
		SHA384Hash();
		unsigned char _data[48];
		operator unsigned char* ();
		std::wstring toString();
		std::string toANSIString();
	};
	struct SHA512Hash {
		SHA512Hash();
		unsigned char _data[64];
		operator unsigned char* ();
		std::wstring toString();
		std::string toANSIString();
	};
	SHA1Hash SHA1EncryptString(const std::wstring& val);
	SHA1Hash SHA1EncryptFile(const std::wstring& path);
	SHA1Hash SHA1Encrypt(const unsigned char* data, size_t byteCount);
	SHA256Hash SHA256EncryptString(const std::wstring& val);
	SHA256Hash SHA256EncryptFile(const std::wstring& path);
	SHA256Hash SHA256Encrypt(const unsigned char* data, size_t byteCount);
	SHA384Hash SHA384EncryptString(const std::wstring& val);
	SHA384Hash SHA384EncryptFile(const std::wstring& path);
	SHA384Hash SHA384Encrypt(const unsigned char* data, size_t byteCount);
	SHA512Hash SHA512EncryptString(const std::wstring& val);
	SHA512Hash SHA512EncryptFile(const std::wstring& path);
	SHA512Hash SHA512Encrypt(const unsigned char* data, size_t byteCount);
};


#pragma once
#include <openssl/rsa.h>
class RSAEncrypt
{
public:
	RSAEncrypt(const std::string& publicKeyPath);
	~RSAEncrypt();
	static void GenerateRSAKeyFile(std::string privateKeyPath, std::string publicKeyPath) noexcept;
	void Encrypt(std::string filePath);
	void Encrypt(std::string filePath, std::string outputFilePath);
	void Encrypt(const unsigned char* source, unsigned char* buffer, int byteCount);
protected:
	std::string publicKeyPath;
	RSA* key;
};
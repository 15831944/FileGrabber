#pragma once
class AESKey
{
public:
	AESKey(const char* keyPath, const char* rsaPrivateFilePath);
	AESKey();
	const unsigned char* GetKey()const;
	const unsigned char* GetIV()const;
	void WriteAESKey(const char* path, const char* rsaPublicFilePath)const;
	static void GenerateRSAEncryptionFile(const char* rsaPublicFilePath, const char* rsaPrivateFilePath);
protected:
	unsigned char key[256];
	unsigned char iv[256];
	void ReadAESKey(const char* keyPath, const char* rsaPrivateFilePath);
	void GenerateAESKey();
};


#pragma once
class RSAEncrypt
{
public:
	RSAEncrypt();
	static void GenerateRSAKeyFile(std::string privateKeyPath, std::string publicKeyPath);
};


#pragma once
#include <string>
typedef struct rsa_st RSA;

class RSAKey
{
public:
	RSAKey();
	~RSAKey();
	void generate();
	void setPublicKey(const std::string& path);
	void setPublicKey(const std::wstring& path);
	void setPrivateKey(const std::string& path);
	void setPrivateKey(const std::wstring& path);
	void writePublicKey(const std::string& path);
	void writePublicKey(const std::wstring& path);
	void writePrivateKey(const std::string& path);
	void writePrivateKey(const std::wstring& path);
	RSA* getRSAKey();
	enum class KeyType {
		NONE,
		PUBLIC,
		PRIVATE,
		ALL,
		GENERATED_INSTANCE
	};
protected:
	RSA *key, *pubkey, *prikey;
	KeyType type;
};


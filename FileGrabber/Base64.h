#pragma once

class Base64
{
public:
	enum class EncodingMode {
		BASE64_WITH_NEWLINE,
		BASE64_NO_NEWLINE
	};

	Base64();
	Base64(EncodingMode mode);
	std::string encodeString(const unsigned char* data, int length) const;
	std::string encodeString(std::wstring& str) const;
	std::string encodeString(std::string& str) const;

	void setEncodingMode(EncodingMode mode);
	EncodingMode getEncodingMode() const;

protected:
	EncodingMode mode;
};


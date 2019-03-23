#pragma once
#include <string>

class Convert
{
public:
	Convert() {}
	std::wstring toString(int var);
	std::wstring toString(uint var);
	std::wstring toString(int64 var);
	std::wstring toString(uint64 var);
	std::wstring toString(double var);
	std::wstring toString(const std::string &var);
	std::string toANSIString(int var);
	std::string toANSIString(uint var);
	std::string toANSIString(int64 var);
	std::string toANSIString(uint64 var);
	std::string toANSIString(double var);
	std::string toANSIString(const std::wstring &var);
	int toInteger(const std::wstring &var);
	int toInteger(const std::string &var);
	uint toUnsignedInteger(const std::wstring &var);
	uint toUnsignedInteger(const std::string &var);
	int64 toInt64(const std::wstring &var);
	int64 toInt64(const std::string &var);
	uint64 toUnsignedInt64(const std::wstring &var);
	uint64 toUnsignedInt64(const std::string &var);
};


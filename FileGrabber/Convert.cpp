#include "stdafx.h"
#include "Convert.h"
using namespace std;

wstring Convert::toString(int var)
{
	wchar_t buffer[13];
	swprintf_s(buffer, 12, L"%d", var);
	wstring str = buffer;
	return str;
}

wstring Convert::toString(uint var)
{
	wchar_t buffer[13];
	swprintf_s(buffer, 12, L"%u", var);
	wstring str = buffer;
	return str;
}

wstring Convert::toString(int64 var)
{
	wchar_t buffer[22];
	swprintf_s(buffer, 21, L"%lld", var);
	wstring str = buffer;
	return str;
}

wstring Convert::toString(uint64 var)
{
	wchar_t buffer[22];
	swprintf_s(buffer, 21, L"%llu", var);
	wstring str = buffer;
	return str;
}

wstring Convert::toString(double var)
{
	wchar_t buffer[101];
	swprintf_s(buffer, 100, L"%lf", var);
	wstring str = buffer;
	return str;
}

wstring Convert::toString(const string &var)
{
	wstring wstr;
	locale loc = locale();
	for (auto const& c : var)
	{
		wchar_t wchar = std::use_facet<ctype<wchar_t>>(loc).widen(c);
		wstr.push_back(wchar);
	}
	return wstr;
}

string Convert::toANSIString(int var)
{
	char buffer[13];
	sprintf_s(buffer, 12, "%d", var);
	string str = buffer;
	return str;
}

string Convert::toANSIString(uint var)
{
	char buffer[13];
	sprintf_s(buffer, 12, "%u", var);
	string str = buffer;
	return str;
}

string Convert::toANSIString(int64 var)
{
	char buffer[22];
	sprintf_s(buffer, 21, "%lld", var);
	string str = buffer;
	return str;
}

string Convert::toANSIString(uint64 var)
{
	char buffer[22];
	sprintf_s(buffer, 21, "%llu", var);
	string str = buffer;
	return str;
}

string Convert::toANSIString(double var)
{
	char buffer[101];
	sprintf_s(buffer, 100, "%lf", var);
	string str = buffer;
	return str;
}

string Convert::toANSIString(const wstring & var)
{
	string str;
	locale loc = locale();
	for (auto const& c : var)
	{
		char ch = std::use_facet<ctype<wchar_t>>(loc).narrow(c);
		str.push_back(ch);
	}
	return str;
}

int Convert::toInteger(const wstring & var)
{
	return _wtoi(var.c_str());
}

int Convert::toInteger(const string & var)
{
	return atoi(var.c_str());
}

uint Convert::toUnsignedInteger(const wstring & var)
{
	wchar_t* temp;
	return (uint)wcstoul(var.c_str(), &temp, 10);
}

uint Convert::toUnsignedInteger(const string & var)
{
	char* temp;
	return (uint)strtoul(var.c_str(), &temp, 10);
}

int64 Convert::toInt64(const wstring & var)
{
	return _wtoi64(var.c_str());
}

int64 Convert::toInt64(const string & var)
{
	return _atoi64(var.c_str());
}

uint64 Convert::toUnsignedInt64(const wstring & var)
{
	wchar_t* temp;
	return (uint64)_wcstoui64(var.c_str(), &temp, 10);
}

uint64 Convert::toUnsignedInt64(const string & var)
{
	char* temp;
	return (uint64)_strtoui64(var.c_str(), &temp, 10);
}
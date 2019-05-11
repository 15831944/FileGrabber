#pragma once
#include <string>

class Log
{
public:
	static Log* getInstance();
	void i(const std::wstring& msg);
	void w(const std::wstring& msg);
	void e(const std::wstring& msg);
	void v(const std::wstring& msg);
	void d(const std::wstring& msg);
private:
	Log();
};

#define LOG Log::getInstance()

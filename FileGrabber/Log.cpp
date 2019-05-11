#include "stdafx.h"
#include "Log.h"
#include <fstream>
#include "ApplicationInformation.h"
using namespace std;

wstring GetTimeString();

constexpr const wchar_t* LogName = L"FileGrabber.log";

namespace {
	Log* plog = nullptr;
}

Log* Log::getInstance()
{
	if (plog == nullptr) {
		plog = new Log();
	}
	return plog;
}

void Log::i(const std::wstring& msg)
{
	wofstream fs;
	fs.open(LogName, ios::app);
	fs << L"[Info](" << GetTimeString() << L"): " << msg << endl;
	fs.close();
}

void Log::w(const std::wstring& msg)
{
	wofstream fs;
	fs.open(LogName, ios::app);
	fs << L"[Warning](" << GetTimeString() << L"): " << msg << endl;
	fs.close();
}

void Log::e(const std::wstring& msg)
{
	wofstream fs;
	fs.open(LogName, ios::app);
	fs << L"[Error](" << GetTimeString() << L"): " << msg << endl;
	fs.close();
}

void Log::v(const std::wstring& msg)
{
	wofstream fs;
	fs.open(LogName, ios::app);
	fs << L"[Verbose](" << GetTimeString() << L"): " << msg << endl;
	fs.close();
}

void Log::d(const std::wstring& msg)
{
	wofstream fs;
	fs.open(LogName, ios::app);
	fs << L"[Debug](" << GetTimeString() << L"): " << msg << endl;
	fs.close();
}

Log::Log() {
	wofstream fs;
	fs.open(LogName, ios::app);
	fs << L"[Version](" << GetTimeString() << L"): FileGrabber " << ApplicationInformation::getVersion() << L" Log System start working." << endl;
	fs.close();
}

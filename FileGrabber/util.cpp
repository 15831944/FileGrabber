#include "stdafx.h"
#include <string>
using namespace std;

wstring SystemTimeToString(const SYSTEMTIME& st) {
	wchar_t buffer[50];
	swprintf_s(buffer, 50, L"%04u-%02u-%02u %02u:%02u:%02u.%03u", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	return buffer;
}
﻿// stdafx.h: 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 项目特定的包含文件
//

#pragma once

#include "targetver.h"

//#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
// Windows 头文件
#include <windows.h>

// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// 在此处引用程序需要的其他标头
#include <string>
namespace std {
#ifdef UNICODE
typedef wstring _tstring;
#else
typedef string _tstring;
#endif
}

typedef unsigned int uint;
typedef long long int64;
typedef unsigned long long uint64;
typedef unsigned long ulong;

#define WM_NOTIFYMSG				WM_USER
#define IDN_SERVICE					1501
#define IDN_EXIT					1502
#define IDN_ENABLECOPY				1503

struct FileData {
	TCHAR Directory[_MAX_PATH];
	FILETIME time_create;
	FILETIME time_access;
	FILETIME time_write;
	DWORD attributes;
	DWORD64 size;
	TCHAR name[_MAX_PATH];
};
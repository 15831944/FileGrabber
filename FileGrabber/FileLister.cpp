#include "stdafx.h"
#include "FileLister.h"
#include <io.h>
#include "Convert.h"
using namespace std;

static void ListFileProcess(shared_ptr<list<FileData>> ptr, const TCHAR* path);


shared_ptr<list<FileData>> FileLister::ListFile()
{
	shared_ptr<list<FileData>> ls = make_shared<list<FileData>>();
	TCHAR path[3] = { device.GetDiskLabel(),TEXT(':'),TEXT('\0') };
	ListFileProcess(ls, path);
	return ls;
}

static void ListFileProcess(shared_ptr<list<FileData>> ptr, const TCHAR* path) {
	TCHAR szFind[MAX_PATH];
	TCHAR szFile[MAX_PATH];
	WIN32_FIND_DATA FindFileData;
	_tcscpy_s(szFind, path);
	_tcscat_s(szFind, TEXT("/*.*"));
	HANDLE hFind = FindFirstFile(szFind, &FindFileData);
	if (INVALID_HANDLE_VALUE == hFind)
		return;
	while (true)
	{
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (FindFileData.cFileName[0] != '.')
			{
				FileData data;
				_tcscpy_s(data.Directory, path);
				_tcscpy_s(data.name, FindFileData.cFileName);
				data.time_access = FindFileData.ftLastAccessTime;
				data.time_create = FindFileData.ftCreationTime;
				data.time_write = FindFileData.ftLastWriteTime;
				data.attributes = FindFileData.dwFileAttributes;
				data.size = (DWORD64(FindFileData.nFileSizeHigh) << 32) + FindFileData.nFileSizeLow;
				ptr->push_back(data);
				_tcscpy_s(szFile, path);
				_tcscat_s(szFile, TEXT("/"));
				_tcscat_s(szFile, FindFileData.cFileName);
				ListFileProcess(ptr, szFile);
			}
		}
		else {
			FileData data;
			_tcscpy_s(data.Directory, path);
			_tcscpy_s(data.name, FindFileData.cFileName);
			data.time_access = FindFileData.ftLastAccessTime;
			data.time_create = FindFileData.ftCreationTime;
			data.time_write = FindFileData.ftLastWriteTime;
			data.attributes = FindFileData.dwFileAttributes;
			data.size = (DWORD64(FindFileData.nFileSizeHigh) << 32) + FindFileData.nFileSizeLow;
			ptr->push_back(data);
		}
		if (!FindNextFile(hFind, &FindFileData))
			break;
	}
	FindClose(hFind);
}
#include "stdafx.h"
#include "FileLister.h"
#include <io.h>
#include "Convert.h"
using namespace std;

void ListFileProcess(shared_ptr<list<FileLister::FileData>> ptr, const TCHAR* path);

FileLister::FileLister(const Device& device) {
	this->device = device;
}

shared_ptr<list<FileLister::FileData>> FileLister::ListFile()
{
	shared_ptr<list<FileData>> ls = make_shared<list<FileData>>();
	TCHAR path[3] = { device.GetDiskLabel(),TEXT(':'),TEXT('\0') };
	ListFileProcess(ls, path);
	return ls;
}

void ListFileProcess(shared_ptr<list<FileLister::FileData>> ptr, const TCHAR* path) {
	TCHAR szFind[MAX_PATH];
	TCHAR szFile[MAX_PATH];
	WIN32_FIND_DATA FindFileData;
	_tcscpy(szFind, path);
	_tcscat(szFind, TEXT("/*.*"));
	HANDLE hFind = FindFirstFile(szFind, &FindFileData);
	if (INVALID_HANDLE_VALUE == hFind)
		return;
	while (true)
	{
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (FindFileData.cFileName[0] != '.')
			{
				_tcscpy(szFile, path);
				_tcscat(szFile, TEXT("/"));
				_tcscat(szFile, FindFileData.cFileName);
				ListFileProcess(ptr, szFile);
			}
		}
		else
		{
			FileLister::FileData data;
			_tcscpy(data.Dictionary, path);
			_tcscpy(data.name, FindFileData.cFileName);
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
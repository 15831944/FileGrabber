#pragma once
#include "Device.h"
#include <memory>
#include <list>

class FileLister
{
public:
	FileLister(const Device& device);
	struct FileData {
		TCHAR Dictionary[_MAX_PATH];
		FILETIME time_create;
		FILETIME time_access;
		FILETIME time_write;
		DWORD attributes;
		DWORD64 size;
		TCHAR name[_MAX_PATH];
	};
	std::shared_ptr<std::list<FileData>> ListFile();
	void SafeListFile(const TCHAR* filename);
protected:
	Device device;
};


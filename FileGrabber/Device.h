#pragma once
#include <string>
class Device
{
public:
	Device(TCHAR diskLabel);
	Device();
	enum class DiskStatus {
		OK,
		INVALID
	};
	struct DiskInformation {
		TCHAR DriveLetter;
		std::_tstring Label;
		unsigned long long TotalSpace;
		unsigned long long FreeSpaceToCaller;
		unsigned long long FreeSpace;
		std::_tstring FileSystem;
		DWORD FileSystemFlags;
		DWORD VolumeSerialNumber;
	};
	TCHAR GetDiskLabel() const;
	void SetDiskStatus(DiskStatus ds);
	DiskStatus GetDiskStatus() const;
	DiskInformation GetDiskInformation();
	SYSTEMTIME GetDiskArriveTime()const;
protected:
	DiskStatus diskStatus;
	TCHAR diskLabel;
	SYSTEMTIME arriveTime;
};


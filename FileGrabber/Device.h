#pragma once
#include <string>
class Device
{
public:
	Device(wchar_t diskLabel);
	Device();
	enum class DiskStatus {
		OK,
		INVALID
	};
	struct DiskInformation {
		wchar_t DriveLetter;
		std::_tstring Label;
		unsigned long long TotalSpace;
		unsigned long long FreeSpaceToCaller;
		unsigned long long FreeSpace;
		std::_tstring FileSystem;
		DWORD FileSystemFlags;
		DWORD VolumeSerialNumber;
	};
	wchar_t GetDiskLabel() const;
	void SetDiskStatus(DiskStatus ds);
	DiskStatus GetDiskStatus() const;
	DiskInformation GetDiskInformation();
	SYSTEMTIME GetDiskArriveTime()const;
protected:
	DiskStatus diskStatus;
	wchar_t diskLabel;
	SYSTEMTIME arriveTime;
};


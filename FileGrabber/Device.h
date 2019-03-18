#pragma once
#include <string>
class Device
{
public:
	Device(TCHAR diskLabel);
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
	inline TCHAR GetDiskLabel() const;
	inline void SetDiskStatus(DiskStatus ds);
	inline DiskStatus GetDiskStatus() const;
	DiskInformation GetDiskInformation();
protected:
	DiskStatus diskStatus;
	TCHAR diskLabel;
};


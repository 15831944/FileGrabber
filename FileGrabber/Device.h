#pragma once
#include <string>
class Device
{
public:
	Device(char diskLabel);
	enum class DiskStatus {
		OK,
		INVALID
	};
	struct DiskInformation {
		char DriveLetter;
		std::wstring Label;
		unsigned int TotalSpace;
		unsigned int UsedSpace;
		unsigned int FreeSpace;
		std::wstring FileSystem;
		std::wstring DiskSerialNumber;
	};
	inline char GetDiskLabel() const;
	inline void SetDiskStatus(DiskStatus ds);
	inline void GetDiskStatus() const;
	const DiskInformation& GetDiskInformation();
protected:
	DiskInformation ReadDiskInformation() const;
	DiskInformation diskInformation;
	DiskStatus diskStatus;
};


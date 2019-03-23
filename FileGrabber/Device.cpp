#include "stdafx.h"
#include "Device.h"

Device::Device(TCHAR diskLabel)
{
	this->diskLabel = diskLabel;
	this->diskStatus = DiskStatus::OK;
}

Device::Device()
{
	this->diskLabel = 0;
	this->diskStatus = DiskStatus::INVALID;
}

TCHAR Device::GetDiskLabel() const
{
	return this->diskLabel;
}

void Device::SetDiskStatus(DiskStatus ds)
{
	this->diskStatus = ds;
}

Device::DiskStatus Device::GetDiskStatus() const
{
	return this->diskStatus;
}

Device::DiskInformation Device::GetDiskInformation()
{
	DiskInformation info;
	if (this->diskStatus == DiskStatus::INVALID) {
		return info;
	}
	TCHAR path[3] = { this->diskLabel,TEXT(':'),TEXT('\0') };
	TCHAR VolumeName[255];
	DWORD VolumeSerialNumber, MaximumComponentLength, FileSystemFlags;
	TCHAR FileSystemName[255];
	GetVolumeInformation(path, VolumeName, 254, &VolumeSerialNumber, &MaximumComponentLength, &FileSystemFlags, FileSystemName, 254);
	info.DriveLetter = this->diskLabel;
	info.FileSystem = FileSystemName;
	info.VolumeSerialNumber = VolumeSerialNumber;
	info.FileSystemFlags = FileSystemFlags;
	info.Label = VolumeName;
	DWORD64 FreeBytesToCaller, TotalBytes, FreeBytes;
	GetDiskFreeSpaceEx(path, (PULARGE_INTEGER)&FreeBytesToCaller, (PULARGE_INTEGER)&TotalBytes, (PULARGE_INTEGER)&FreeBytes);
	info.FreeSpace = FreeBytes;
	info.FreeSpaceToCaller = FreeBytesToCaller;
	info.TotalSpace = TotalBytes;
	return info;
}

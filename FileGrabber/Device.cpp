#include "stdafx.h"
#include "Device.h"
#include <clocale>

Device::Device(wchar_t diskLabel)
{
	this->diskLabel = diskLabel;
	this->diskStatus = DiskStatus::OK;
	_wsetlocale(LC_ALL, L"chs");
	GetLocalTime(&this->arriveTime);
}

Device::Device()
{
	this->diskLabel = 0;
	this->diskStatus = DiskStatus::INVALID;
}

wchar_t Device::GetDiskLabel() const
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
	wchar_t path[3] = { this->diskLabel, L':', L'\0' };
	wchar_t VolumeName[255];
	DWORD VolumeSerialNumber, MaximumComponentLength, FileSystemFlags;
	wchar_t FileSystemName[255];
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

SYSTEMTIME Device::GetDiskArriveTime() const
{
	return arriveTime;
}

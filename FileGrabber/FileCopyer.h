#pragma once
#include "Device.h"
#include <memory>
#include <list>

class FileCopyer
{
public:
	FileCopyer(const Device& device);
	FileCopyer(const Device& device, std::shared_ptr<std::list<FileData>> paths);
	void setPaths(std::shared_ptr<std::list<FileData>> paths);
	void ListFile(std::shared_ptr<std::list<FileData>> plist);
	void Copy();
	void Encrypt();
	void Compress();
protected:
	Device device;
	std::shared_ptr<std::list<FileData>> paths;
	//AESKey key;
};


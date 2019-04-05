#pragma once
#include "Device.h"
#include <memory>
#include <list>

class FileLister
{
public:
	FileLister(const Device& device) : device(device) {}
	std::shared_ptr<std::list<FileData>> ListFile();
protected:
	Device device;
};


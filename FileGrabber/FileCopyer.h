#pragma once
#include "Device.h"
#include <memory>
#include <list>

class FileCopyer
{
public:
	FileCopyer(const Device& device);
	void Copy(std::shared_ptr<std::list<FileData>> paths);
	void Encrypt();
	void Compress();
protected:
	Device device;
};


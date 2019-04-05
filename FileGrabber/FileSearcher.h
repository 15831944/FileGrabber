#pragma once
#include <memory>
#include <list>
#include <vector>
#include "Device.h"

class FileSearcher
{
public:
	FileSearcher() {}
	std::shared_ptr<std::list<FileData>> Find(std::shared_ptr<std::list<FileData>> plist, std::vector<std::_tstring> filters,
		bool icase = false)const;
	std::shared_ptr<std::list<FileData>> FindRegex(std::shared_ptr<std::list<FileData>> plist, std::vector<std::_tstring> regexStrs) const;
	std::shared_ptr<std::list<FileData>> FindDirectory(std::shared_ptr<std::list<FileData>> plist, std::vector<std::_tstring> filters,
		bool icase = false)const;
	std::shared_ptr<std::list<FileData>> FindDirectoryRegex(std::shared_ptr<std::list<FileData>> plist, std::vector<std::_tstring> regexStrs) const;
	std::shared_ptr<std::list<FileData>> FindFile(std::shared_ptr<std::list<FileData>> plist, std::vector<std::_tstring> filters,
		bool icase = false)const;
	std::shared_ptr<std::list<FileData>> FindFileRegex(std::shared_ptr<std::list<FileData>> plist, std::vector<std::_tstring> regexStrs) const;
};


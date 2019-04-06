#pragma once
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <vector>

class SystemConfig
{
public:
	static SystemConfig* getInstance();
	void ReadConfig();
	std::string PublicKeyPath;
	bool FileCopyer, ListenSN, NormalCopy, RegexCopy;
	std::vector<std::_tstring> NormalCopyFilters, RegexCopyFilters;
	std::vector<unsigned int> SNFilter;
protected:
	SystemConfig();
	xercesc::DOMNode* findChildNode(xercesc::DOMNode* node, const char* name);
	bool CompareVersion(int major, int minor, int fix, int mmajor, int mminor, int mfix);
};


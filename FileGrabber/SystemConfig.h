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
	enum class SNMode {
		DISABLED,
		EXCEPTATIONS,
		LISTENLIST
	}ListenSN;
	bool FileCopyer, NormalCopy, RegexCopy;
	std::vector<std::wstring> NormalCopyFilters, RegexCopyFilters;
	std::vector<unsigned int> SNFilter;
protected:
	SystemConfig();
	xercesc::DOMNode* findChildNode(xercesc::DOMNode* node, const char* name);
	bool CompareVersion(int major, int minor, int fix, int mmajor, int mminor, int mfix);
};


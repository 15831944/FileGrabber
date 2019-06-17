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
	void loadConfig();
	std::string PublicKeyPath;
	enum class SNMode {
		DISABLED,
		EXCEPTATIONS,
		LISTENLIST
	}ListenSN;
	bool FileCopyer, NormalCopy, RegexCopy;
	std::vector<std::wstring> NormalCopyFilters, RegexCopyFilters;
	std::vector<unsigned int> SNFilter;
	uint64 LimitSize;
	uint LimitCount;
protected:
	struct ConfigData {
		struct {

		} ConfigFileInformation;

		struct {
			int Major;
			int Minor;
			int Fix;
		} MinimumVersion;

		struct {

		} Encryption;
	};
	SystemConfig();
	bool CompareVersion(int major, int minor, int fix, int mmajor, int mminor, int mfix);
};


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
	enum class ListenMode {
		LISTEN,
		DISABLED
	};
	struct ConfigData {
		struct {
			int Major;
			int Minor;
			int Fix;
		} MinimumVersion;

		struct {
			bool Enabled;
			std::string Path;
		} Encryption;

		struct DeviceConfig {
			std::string Name;
			unsigned int SerialNumber;
			ListenMode ListenMode;
			struct {
				bool LimitEnbaled;
				bool MaxSizeLimited;
				uint64 MaxSize;
				bool MaxCountLimited;
				uint64 MaxCount;
			} Limit;
			bool FileListerEnabled;
			struct {
				bool NormalCopyEnabled;
				std::vector<std::wstring> NormalFilters;
				bool RegexCopyEnabled;
				std::vector<std::wstring> RegexFilters;
			} FileCopyer;
		};

		struct {
			bool Enabled;
			DeviceConfig DefaultConfig;
			std::vector<DeviceConfig> DeviceConfig;
		} Service;
	};
	ConfigData readConfig();
protected:
	SystemConfig();
	bool CompareVersion(int major, int minor, int fix, int mmajor, int mminor, int mfix);
};
#include "stdafx.h"
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include "SystemConfig.h"
#include "ApplicationInformation.h"
#include "Convert.h"
#include "XMLParseError.h"
#include <stdexcept>
#include <regex>
#include <set>
#include <filesystem>
using namespace std;
using namespace xercesc;

static SystemConfig* psc = nullptr;

// If statement is false, it will throw an error.
inline bool CHECK_INV_IF(bool statement) {
	if (statement) {
		return true;
	}
	else {
		throw runtime_error("FileGrabber Configure Error: Invalid XML Config File.");
		return false;
	}
}

// If statement is false, it will throw an error.
inline bool CHECK_INV_IF(bool statement, const string& message) {
	if (statement) {
		return true;
	}
	else {
		throw runtime_error(message);
		return false;
	}
}

SystemConfig* SystemConfig::getInstance()
{
	if (psc == nullptr) {
		psc = new SystemConfig();
	}
	return psc;
}

static bool StringToBoolean(string s) {
	if (s == "true") {
		return true;
	}
	else {
		return false;
	}
}

SystemConfig::ConfigData SystemConfig::readConfig()
{
	XMLPlatformUtils::Initialize();
	XercesDOMParser* parser = new XercesDOMParser();
	parser->setValidationScheme(XercesDOMParser::Val_Always);
	parser->setDoNamespaces(false);
	ErrorHandler* errHandler = (ErrorHandler*)new HandlerBase();
	parser->setErrorHandler(errHandler);
	parser->parse("./config.xml");
	xercesc::DOMDocument* doc = parser->getDocument();
	DOMElement* root = doc->getDocumentElement();

	// 0. Preparing works.
	char* buffer = nullptr;
	bool flag = false;
	ConfigData data;
	Convert convert;

	// 1. Check configure information.
	DOMNodeList* ConfigInformationList = root->getElementsByTagName(u"ConfigInformation");
	CHECK_INV_IF(ConfigInformationList->getLength() == 1);
	DOMElement* ConfigInformation = static_cast<DOMElement*>(ConfigInformationList->item(0));
	DOMNodeList* MinimumVersionList = ConfigInformation->getElementsByTagName(u"MinimumVersion");
	CHECK_INV_IF(MinimumVersionList->getLength() == 1);
	DOMElement* MinimumVersion = static_cast<DOMElement*>(MinimumVersionList->item(0));
	buffer = XMLString::transcode(MinimumVersion->getTextContent());
	CHECK_INV_IF(sscanf_s(buffer, "%d.%d.%d", &data.MinimumVersion.Major, &data.MinimumVersion.Minor, &data.MinimumVersion.Fix) == 3);
	XMLString::release(&buffer);
	int mmajor, mminor, mfix;
	CHECK_INV_IF(swscanf_s(FG_VERSION_PTR, L"%d.%d.%d", &mmajor, &mminor, &mfix) == 3, "Fatal Error: Invalid version number.");
	CHECK_INV_IF(CompareVersion(mmajor, mminor, mfix, data.MinimumVersion.Major, data.MinimumVersion.Minor, data.MinimumVersion.Fix),
		"FileGrabber Configure Error: Minimum version requied is higher than the current version.");

	// 2. Read encryption settings.
	DOMNodeList* EncryptionList = root->getElementsByTagName(u"Encryption");
	CHECK_INV_IF(EncryptionList->getLength() == 1);
	DOMElement* Encryption = static_cast<DOMElement*>(EncryptionList->item(0));
	buffer = XMLString::transcode(Encryption->getAttribute(u"enabled"));
	if (strcmp(buffer, "") == 0)
		flag = true;
	else if (strcmp(buffer, "true") == 0)
		flag = true;
	else
		flag = false;
	XMLString::release(&buffer);
	data.Encryption.Enabled = flag;
	if (flag) {
		DOMNodeList* EncryptionPublicKeyPathList = Encryption->getElementsByTagName(u"PublicKey");
		CHECK_INV_IF(EncryptionPublicKeyPathList->getLength() == 1);
		DOMElement* EncryptionPublicKeyPath = static_cast<DOMElement*>(EncryptionPublicKeyPathList->item(0));
		buffer = XMLString::transcode(EncryptionPublicKeyPath->getTextContent());
		string path = buffer;
		XMLString::release(&buffer);
		CHECK_INV_IF(filesystem::exists(path), "FileGrabber IO Error: Cannot find public key file.");
		data.Encryption.Path = path;
	}

	// 3. Read Configuration.
	// 3.0 Preparing works.
	DOMNodeList* ServiceList = root->getElementsByTagName(u"Service");
	CHECK_INV_IF(ServiceList->getLength() == 1);
	DOMElement* Service = static_cast<DOMElement*>(ServiceList->item(0));
	buffer = XMLString::transcode(Service->getAttribute(u"enabled"));
	if (strcmp(buffer, "") == 0)
		flag = true;
	else if (strcmp(buffer, "true") == 0)
		flag = true;
	else
		flag = false;
	XMLString::release(&buffer);
	data.Service.Enabled = flag;

	// 3.1 Default Configuration.
	if (data.Service.Enabled) {
		DOMNodeList* DefaultConfigList = Service->getElementsByTagName(u"DefaultConfig");
		CHECK_INV_IF(DefaultConfigList->getLength() == 1);
		DOMElement* DefaultConfig = static_cast<DOMElement*>(DefaultConfigList->item(0));
		data.Service.DefaultConfig.Name = "Not available";
		data.Service.DefaultConfig.SerialNumber = 0u;

		// 3.1.1 Judge Default Grab Mode.
		buffer = XMLString::transcode(DefaultConfig->getAttribute(u"mode"));
		if (strcmp(buffer, "listen") == 0) {
			data.Service.DefaultConfig.ListenMode = ListenMode::LISTEN;
		}
		else if (strcmp(buffer, "disabled") == 0) {
			data.Service.DefaultConfig.ListenMode = ListenMode::DISABLED;
		}
		else {
			throw runtime_error("FileGrabber Configure Error: Invalid XML Config File.");
		}
		XMLString::release(&buffer);

		// 3.1.2 Read Limit Information.
		DOMNodeList* LimitList = DefaultConfig->getElementsByTagName(u"Limit");
		CHECK_INV_IF(LimitList->getLength() <= 1);
		if (LimitList->getLength() == 0) {
			data.Service.DefaultConfig.Limit.LimitEnbaled = false;
			data.Service.DefaultConfig.Limit.MaxCountLimited = false;
			data.Service.DefaultConfig.Limit.MaxCount = UINT64_MAX;
			data.Service.DefaultConfig.Limit.MaxSizeLimited = false;
			data.Service.DefaultConfig.Limit.MaxSize = UINT64_MAX;
		}
		else {
			DOMElement* Limit = static_cast<DOMElement*>(LimitList->item(0));
			buffer = XMLString::transcode(Limit->getAttribute(u"enabled"));
			if (strcmp(buffer, "") == 0 || strcmp(buffer, "true") == 0) {
				flag = true;
			}
			else {
				flag = false;
			}
			XMLString::release(&buffer);
			if (flag) {
				data.Service.DefaultConfig.Limit.LimitEnbaled = true;
				DOMNodeList* MaxSizeList = Limit->getElementsByTagName(u"MaxSize");
				CHECK_INV_IF(MaxSizeList->getLength() <= 1);
				if (MaxSizeList->getLength() == 0) {
					data.Service.DefaultConfig.Limit.MaxSizeLimited = false;
					data.Service.DefaultConfig.Limit.MaxSize = UINT64_MAX;
				}
				else {
					DOMElement* MaxSize = static_cast<DOMElement*>(MaxSizeList->item(0));
					buffer = XMLString::transcode(MaxSize->getAttribute(u"enabled"));
					if (strcmp(buffer, "") == 0 || strcmp(buffer, "true") == 0) {
						flag = true;
					}
					else {
						flag = false;
					}
					XMLString::release(&buffer);

					if (flag) {
						buffer = XMLString::transcode(MaxSize->getTextContent());
						data.Service.DefaultConfig.Limit.MaxSizeLimited = true;
						data.Service.DefaultConfig.Limit.MaxSize = convert.toUnsignedInt64(buffer);
						XMLString::release(&buffer);
					}
					else {
						data.Service.DefaultConfig.Limit.MaxSizeLimited = false;
						data.Service.DefaultConfig.Limit.MaxSize = UINT64_MAX;
					}
				}

				DOMNodeList* MaxCountList = Limit->getElementsByTagName(u"MaxCount");
				CHECK_INV_IF(MaxCountList->getLength() <= 1);
				if (MaxCountList->getLength() == 0) {
					data.Service.DefaultConfig.Limit.MaxCountLimited = false;
					data.Service.DefaultConfig.Limit.MaxCount = UINT64_MAX;
				}
				else {
					DOMElement* MaxCount = static_cast<DOMElement*>(MaxCountList->item(0));
					buffer = XMLString::transcode(MaxCount->getAttribute(u"enabled"));
					if (strcmp(buffer, "") == 0 || strcmp(buffer, "true") == 0) {
						flag = true;
					}
					else {
						flag = false;
					}
					XMLString::release(&buffer);

					if (flag) {
						buffer = XMLString::transcode(MaxCount->getTextContent());
						data.Service.DefaultConfig.Limit.MaxCountLimited = true;
						data.Service.DefaultConfig.Limit.MaxCount = convert.toUnsignedInt64(buffer);
						XMLString::release(&buffer);
					}
					else {
						data.Service.DefaultConfig.Limit.MaxCountLimited = false;
						data.Service.DefaultConfig.Limit.MaxCount = UINT64_MAX;
					}
				}
			}
			else {
				data.Service.DefaultConfig.Limit.LimitEnbaled = false;
				data.Service.DefaultConfig.Limit.MaxCountLimited = false;
				data.Service.DefaultConfig.Limit.MaxCount = UINT64_MAX;
				data.Service.DefaultConfig.Limit.MaxSizeLimited = false;
				data.Service.DefaultConfig.Limit.MaxSize = UINT64_MAX;
			}
		}

		// 3.1.3 Read FileLister Configure.
		DOMNodeList* FileListerList = DefaultConfig->getElementsByTagName(u"FileLister");
		CHECK_INV_IF(FileListerList->getLength() <= 1);
		switch (FileListerList->getLength()) {
		case 0:
			data.Service.DefaultConfig.FileListerEnabled = true;
			break;
		case 1:
			buffer = XMLString::transcode(static_cast<DOMElement*>(FileListerList->item(0))->getAttribute(u"enabled"));
			if (strcmp(buffer, "") == 0 || strcmp(buffer, "true") == 0) {
				data.Service.DefaultConfig.FileListerEnabled = true;
			}
			else {
				data.Service.DefaultConfig.FileListerEnabled = false;
			}
			XMLString::release(&buffer);
			break;
		}

		// 3.1.4 Read FileCopyer Configure.
		DOMNodeList* FileCopyerList = DefaultConfig->getElementsByTagName(u"FileCopyer");
		CHECK_INV_IF(FileCopyerList->getLength() <= 1);
		if (FileCopyerList->getLength() == 0) {
			data.Service.DefaultConfig.FileCopyer.NormalCopyEnabled = false;
			data.Service.DefaultConfig.FileCopyer.RegexCopyEnabled = false;
		}
		else {
			DOMElement* FileCopyer = static_cast<DOMElement*>(FileCopyerList->item(0));
			buffer = XMLString::transcode(FileCopyer->getAttribute(u"enabled"));
			if (strcmp(buffer, "") == 0 || strcmp(buffer, "true") == 0) {
				flag = true;
			}
			else {
				flag = false;
			}
			XMLString::release(&buffer);

			if (flag) {
				DOMNodeList* NormalCopyList = FileCopyer->getElementsByTagName(u"NormalCopy");
				CHECK_INV_IF(NormalCopyList->getLength() <= 1);
				if (NormalCopyList->getLength() == 0) {
					data.Service.DefaultConfig.FileCopyer.NormalCopyEnabled = false;
				}
				else {
					DOMElement* NormalCopy = static_cast<DOMElement*>(NormalCopyList->item(0));
					buffer = XMLString::transcode(NormalCopy->getAttribute(u"enabled"));
					if (strcmp(buffer, "") == 0 || strcmp(buffer, "true") == 0) {
						flag = true;
					}
					else {
						flag = false;
					}
					XMLString::release(&buffer);

					if (flag) {
						DOMNodeList* NormalFiltersList = NormalCopy->getElementsByTagName(u"Normal");
						size_t count = NormalFiltersList->getLength();
						if (count == 0) {
							data.Service.DefaultConfig.FileCopyer.NormalCopyEnabled = false;
						}
						else {
							data.Service.DefaultConfig.FileCopyer.NormalCopyEnabled = true;
							for (size_t i = 0; i < count; ++i) {
								buffer = XMLString::transcode(NormalFiltersList->item(i)->getTextContent());
								data.Service.DefaultConfig.FileCopyer.NormalFilters.push_back(buffer);
								XMLString::release(&buffer);
							}
						}
					}
					else {
						data.Service.DefaultConfig.FileCopyer.NormalCopyEnabled = false;
					}
				}

				DOMNodeList* RegexCopyList = DefaultConfig->getElementsByTagName(u"RegexCopy");
				CHECK_INV_IF(RegexCopyList->getLength() <= 1);
				if (RegexCopyList->getLength() == 0) {
					data.Service.DefaultConfig.FileCopyer.RegexCopyEnabled = false;
				}
				else {
					DOMElement* RegexCopy = static_cast<DOMElement*>(RegexCopyList->item(0));
					buffer = XMLString::transcode(RegexCopy->getAttribute(u"enabled"));
					if (strcmp(buffer, "") == 0 || strcmp(buffer, "true") == 0) {
						flag = true;
					}
					else {
						flag = false;
					}
					XMLString::release(&buffer);

					if (flag) {
						data.Service.DefaultConfig.FileCopyer.RegexCopyEnabled = false;
					}
					else {
						DOMNodeList* RegexFiltersList = RegexCopy->getElementsByTagName(u"Regex");
						size_t count = RegexFiltersList->getLength();
						if (count == 0) {
							data.Service.DefaultConfig.FileCopyer.RegexCopyEnabled = false;
						}
						else {
							data.Service.DefaultConfig.FileCopyer.RegexCopyEnabled = true;
							for (size_t i = 0; i < count; ++i) {
								buffer = XMLString::transcode(RegexFiltersList->item(i)->getTextContent());
								data.Service.DefaultConfig.FileCopyer.RegexCopyFilters.push_back(buffer);
								XMLString::release(&buffer);
							}
						}
					}
				}

			}
			else {
				data.Service.DefaultConfig.FileCopyer.NormalCopyEnabled = false;
				data.Service.DefaultConfig.FileCopyer.RegexCopyEnabled = false;
			}
		}
	}

	// 3.2 Device Configuration.
	if (data.Service.Enabled) {
		DOMNodeList* DeviceConfigList = Service->getElementsByTagName(u"Device");
		set<uint> SerialNumbers;
		set<string> DeviceNames;
		for (size_t cur = 0; cur < DeviceConfigList->getLength(); ++cur) {
			ConfigData::DeviceConfig cfg;
			DOMElement* DeviceConfig = static_cast<DOMElement*>(DeviceConfigList->item(cur));
			buffer = XMLString::transcode(DeviceConfig->getAttribute(u"enabled"));
			if (strcmp(buffer, "") == 0 || strcmp(buffer, "true") == 0) {
				flag = true;
			}
			else {
				flag = false;
			}
			XMLString::release(&buffer);

			if (flag) {
				// 3.2.1 Basic Information.
				buffer = XMLString::transcode(DeviceConfig->getAttribute(u"SerialNumber"));
				CHECK_INV_IF(strcmp(buffer, "") != 0);
				uint SerialNumber = convert.toUnsignedInteger(buffer);
				XMLString::release(&buffer);
				CHECK_INV_IF(SerialNumbers.find(SerialNumber) == SerialNumbers.end());
				SerialNumbers.insert(SerialNumber);
				cfg.SerialNumber = SerialNumber;
				buffer = XMLString::transcode(DeviceConfig->getAttribute(u"name"));
				cfg.Name = buffer;
				XMLString::release(&buffer);
				if (!cfg.Name.empty()) {
					CHECK_INV_IF(DeviceNames.find(cfg.Name) == DeviceNames.end());
					DeviceNames.insert(cfg.Name);
				}
				buffer = XMLString::transcode(DeviceConfig->getAttribute(u"mode"));
				if (strcmp(buffer, "") == 0 || strcmp(buffer, "listen") == 0) {
					cfg.ListenMode = ListenMode::LISTEN;
				}
				else {
					cfg.ListenMode = ListenMode::DISABLED;
				}
				XMLString::release(&buffer);

				// 3.2.2 Read Limit Information.
				DOMNodeList* LimitList = DeviceConfig->getElementsByTagName(u"Limit");
				CHECK_INV_IF(LimitList->getLength() <= 1);
				if (LimitList->getLength() == 0) {
					cfg.Limit.LimitEnbaled = data.Service.DefaultConfig.Limit.LimitEnbaled;
					cfg.Limit.MaxCountLimited = data.Service.DefaultConfig.Limit.MaxCountLimited;
					cfg.Limit.MaxCount = data.Service.DefaultConfig.Limit.MaxCount;
					cfg.Limit.MaxSizeLimited = data.Service.DefaultConfig.Limit.MaxSizeLimited;
					cfg.Limit.MaxSize = data.Service.DefaultConfig.Limit.MaxSize;
				}
				else {
					DOMElement* Limit = static_cast<DOMElement*>(LimitList->item(0));
					buffer = XMLString::transcode(Limit->getAttribute(u"enabled"));
					int iflag = 0;
					if (strcmp(buffer, "") == 0 || strcmp(buffer, "true") == 0) {
						iflag = 2;
					}
					else if (strcmp(buffer, "false") == 0) {
						iflag = 1;
					}
					else {
						iflag = 0;
					}
					XMLString::release(&buffer);
					if (iflag==2) {
						cfg.Limit.LimitEnbaled = true;
						DOMNodeList* MaxSizeList = Limit->getElementsByTagName(u"MaxSize");
						CHECK_INV_IF(MaxSizeList->getLength() <= 1);
						if (MaxSizeList->getLength() == 0) {
							cfg.Limit.MaxSizeLimited = false;
							cfg.Limit.MaxSize = UINT64_MAX;
						}
						else {
							DOMElement* MaxSize = static_cast<DOMElement*>(MaxSizeList->item(0));
							buffer = XMLString::transcode(MaxSize->getAttribute(u"enabled"));
							if (strcmp(buffer, "") == 0 || strcmp(buffer, "true") == 0) {
								flag = true;
							}
							else {
								flag = false;
							}
							XMLString::release(&buffer);

							if (flag) {
								buffer = XMLString::transcode(MaxSize->getTextContent());
								cfg.Limit.MaxSizeLimited = true;
								cfg.Limit.MaxSize = convert.toUnsignedInt64(buffer);
								XMLString::release(&buffer);
							}
							else {
								cfg.Limit.MaxSizeLimited = false;
								cfg.Limit.MaxSize = UINT64_MAX;
							}
						}

						DOMNodeList* MaxCountList = Limit->getElementsByTagName(u"MaxCount");
						CHECK_INV_IF(MaxCountList->getLength() <= 1);
						if (MaxCountList->getLength() == 0) {
							cfg.Limit.MaxCountLimited = false;
							cfg.Limit.MaxCount = UINT64_MAX;
						}
						else {
							DOMElement* MaxCount = static_cast<DOMElement*>(MaxCountList->item(0));
							buffer = XMLString::transcode(MaxCount->getAttribute(u"enabled"));
							if (strcmp(buffer, "") == 0 || strcmp(buffer, "true") == 0) {
								flag = true;
							}
							else {
								flag = false;
							}
							XMLString::release(&buffer);

							if (flag) {
								buffer = XMLString::transcode(MaxCount->getTextContent());
								cfg.Limit.MaxCountLimited = true;
								cfg.Limit.MaxCount = convert.toUnsignedInt64(buffer);
								XMLString::release(&buffer);
							}
							else {
								cfg.Limit.MaxCountLimited = false;
								cfg.Limit.MaxCount = UINT64_MAX;
							}
						}
					}
					else {
						cfg.Limit.LimitEnbaled = false;
						cfg.Limit.MaxCountLimited = false;
						cfg.Limit.MaxCount = UINT64_MAX;
						cfg.Limit.MaxSizeLimited = false;
						cfg.Limit.MaxSize = UINT64_MAX;
					}
				}

				data.Service.DeviceConfig.push_back(cfg);
			}
		}
	}

	// Final: Clean works.
	XMLPlatformUtils::Terminate();
	return data;
}

SystemConfig::SystemConfig()
{

}

bool SystemConfig::CompareVersion(int major, int minor, int fix, int mmajor, int mminor, int mfix)
{
	if (major > mmajor) {
		return true;
	}
	else if (major < mmajor) {
		return false;
	}
	else {
		if (minor > mminor) {
			return true;
		}
		else if (minor < mminor) {
			return false;
		}
		else {
			if (fix >= mfix) {
				return true;
			}
			else {
				return false;
			}
		}
	}
}

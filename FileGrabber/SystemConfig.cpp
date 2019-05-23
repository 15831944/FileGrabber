#include "stdafx.h"
#include "SystemConfig.h"
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include "ApplicationInformation.h"
#include <stdexcept>
#include "Convert.h"
#include "XMLParseError.h"
using namespace std;
using namespace xercesc;

static SystemConfig* psc = nullptr;


SystemConfig* SystemConfig::getInstance()
{
	if (psc == nullptr) {
		psc = new SystemConfig();
	}
	return psc;
}

inline void InvalidXMLError() {
	throw runtime_error("FileGrabber Configure Error: Invalid XML Config File.");
}

static bool StringToBoolean(string s) {
	if (s == "true") {
		return true;
	}
	else {
		return false;
	}
}

void SystemConfig::loadConfig()
{
	SNFilter.clear();
	NormalCopyFilters.clear();
	RegexCopyFilters.clear();

	Convert convert;
	char* buffer = nullptr;

	XMLPlatformUtils::Initialize();
	XercesDOMParser* parser = new XercesDOMParser();
	parser->setValidationScheme(XercesDOMParser::Val_Always);
	parser->setDoNamespaces(false);
	ErrorHandler* errHandler = (ErrorHandler*)new HandlerBase();
	parser->setErrorHandler(errHandler);
	parser->parse("./config.xml");
	xercesc::DOMDocument* doc = parser->getDocument();
	DOMElement* root = doc->getDocumentElement();

	// 1. Check the minimum version, etc.
	DOMNodeList* ConfigInformationNodes = root->getElementsByTagName(XMLString::transcode("ConfigInformation"));
	if (ConfigInformationNodes->getLength() != 1) {
		throw XMLParseError("Invalid XML Configuration File.");
	}
	DOMNode* ConfigInformationNode = ConfigInformationNodes->item(0);

	XMLPlatformUtils::Terminate();
}

SystemConfig::SystemConfig()
{
	loadConfig();
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

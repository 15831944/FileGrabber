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
#include <regex>
using namespace std;
using namespace xercesc;

static SystemConfig* psc = nullptr;
DOMNode* findFirstChildNode(DOMNode *n, char *nodename);

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

	// 1. Check the minimum version, etc.
	DOMNode* ConfigInformationNode = findFirstChildNode(root, "ConfigInformation");
	if (ConfigInformationNode == nullptr) {
		throw XMLParseError("Invalid XML Configuration File.");
	}
	DOMNode* MinimumVersionNode = findFirstChildNode(ConfigInformationNode, "MinimumVersion");
	if (MinimumVersionNode == nullptr) {
		throw XMLParseError("Invalid XML Configuration File.");
	}
	buffer = XMLString::transcode(MinimumVersionNode->getNodeValue());
	int major, minor, fix;
	if (sscanf_s(buffer, "%d.%d.%d", &major, &minor, &fix) != 3) {
		XMLString::release(&buffer);
		throw XMLParseError("Invalid XML Configuration File.");
	}
	XMLString::release(&buffer);
	int mmajor, mminor, mfix;
	if (swscanf_s(FG_VERSION_PTR, L"%d.%d.%d", &mmajor, &mminor, &mfix) != 3) {
		throw XMLParseError("Invalid XML Configuration File.");
	}
	if (!CompareVersion(major, minor, fix, mmajor, mminor, mfix)) {
		throw XMLParseError("Unsupported XML Configuration File: The minimum version required by the configuration information is higher than the current version.");
	}

	// 2. Read encryption configuration information.
	DOMNode* EncryptionNode = findFirstChildNode(root, "Encryption");
	if (EncryptionNode == nullptr) {
		throw XMLParseError("Invalid XML Configuration File.");
	}
	DOMNode* EncryptionStatusNode = EncryptionNode->getAttributes()->getNamedItem(XMLString::transcode("enabled"));
	if (EncryptionStatusNode == nullptr) {
		
	}
	// Final: Clean works.
	XMLPlatformUtils::Terminate();
}

SystemConfig::SystemConfig()
{
	loadConfig();
}

static DOMNode* findFirstChildNode(DOMNode *n, const char *nodename)
{
	for (DOMNode *child = n->getFirstChild(); child != nullptr; child = child->getNextSibling())
	{
		if (child->getNodeType() == DOMNode::ELEMENT_NODE && XMLString::compareString(child->getNodeName(), XMLString::transcode(nodename)) == 0)
		{
			return child;
		}
	}
	return nullptr;
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

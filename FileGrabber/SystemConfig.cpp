#include "stdafx.h"
#include "SystemConfig.h"
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include "ApplicationInformation.h"
#include <stdexcept>
#include "Convert.h"
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

void SystemConfig::ReadConfig()
{
	Convert convert;
	char* buffer = nullptr;
	XMLPlatformUtils::Initialize();
	XercesDOMParser* parser = new XercesDOMParser();
	parser->setValidationScheme(XercesDOMParser::Val_Always);
	parser->setDoNamespaces(true);
	ErrorHandler* errHandler = (ErrorHandler*)new HandlerBase();
	parser->setErrorHandler(errHandler);
	parser->parse("./config.xml");
	xercesc::DOMDocument* doc = parser->getDocument();
	DOMElement* root = doc->getDocumentElement();

	DOMNode* cinfo = findChildNode(root, "ConfigInformation");
	if (cinfo == nullptr)
		InvalidXMLError();
	DOMNode* minVerNode = findChildNode(cinfo, "MinimumVersion");
	if (minVerNode == nullptr)
		InvalidXMLError();
	buffer = XMLString::transcode(minVerNode->getTextContent());
	string minVersion = buffer;
	XMLString::release(&buffer);
	int mmajor, mminor, mfix, major, minor, fix;
	sscanf_s(minVersion.c_str(), "%d.%d.%d", &mmajor, &mminor, &mfix);
	sscanf_s(ApplicationInformation::getANSIVersion().c_str(), "%d.%d.%d", &major, &minor, &fix);
	if (!CompareVersion(major, minor, fix, mmajor, mminor, mfix)) {
		throw runtime_error("FileGrabber Configure Error: Config file needs higher version of FileGrabber.");
	}
	DOMNode* encryption = findChildNode(root, "Encryption");
	if (encryption == nullptr)
		InvalidXMLError();
	DOMNode* PublicKeyPath = findChildNode(encryption, "PublicKeyPath");
	if (PublicKeyPath == nullptr)
		InvalidXMLError();
	buffer = XMLString::transcode(PublicKeyPath->getTextContent());
	this->PublicKeyPath = buffer;
	XMLString::release(&buffer);

	DOMNode* copyer = findChildNode(root, "FileCopyer");
	buffer = XMLString::transcode(copyer->getAttributes()->getNamedItem(XMLString::transcode("status"))->getTextContent());
	bool enableCopyer = StringToBoolean(buffer);
	XMLString::release(&buffer);

	this->FileCopyer = enableCopyer;
	if (enableCopyer) {
		DOMNode* SNNode = findChildNode(copyer, "SN");
		if (SNNode != nullptr) {
			DOMNode* SNModeNode = SNNode->getAttributes()->getNamedItem(XMLString::transcode("status"));
			SNMode mode;
			if (SNModeNode == nullptr) {
				mode = SNMode::LISTENLIST;
			}
			else {
				buffer = XMLString::transcode(SNModeNode->getTextContent());
				if (strcmp(buffer, "exceptations") == 0) {
					mode = SNMode::EXCEPTATIONS;
				}
				else if (strcmp(buffer, "disabled") == 0) {
					mode = SNMode::DISABLED;
				}
				else {
					mode = SNMode::LISTENLIST;
				}
				XMLString::release(&buffer);
			}
			if (mode != SNMode::DISABLED) {
				for (DOMNode* node = SNNode->getFirstChild(); node != nullptr; node = node->getNextSibling()) {
					if (node->getNodeType() == DOMNode::ELEMENT_NODE) {
						buffer = XMLString::transcode(node->getTextContent());
						SNFilter.push_back(convert.toUnsignedInteger(buffer));
						XMLString::release(&buffer);
					}
				}
			}
			this->ListenSN = mode;
		}
		else {
			this->ListenSN = SNMode::EXCEPTATIONS;
		}

		DOMNode* NormalNode = findChildNode(copyer, "Normal");
		if (NormalNode != nullptr) {
			buffer = XMLString::transcode(NormalNode->getAttributes()->getNamedItem(XMLString::transcode("status"))->getTextContent());
			this->NormalCopy = StringToBoolean(buffer);
			XMLString::release(&buffer);
			if (this->NormalCopy) {
				for (DOMNode* node = NormalNode->getFirstChild(); node != nullptr; node = node->getNextSibling()) {
					if (node->getNodeType() == DOMNode::ELEMENT_NODE) {
						buffer = XMLString::transcode(node->getTextContent());
						NormalCopyFilters.push_back(convert.toString(buffer));
						XMLString::release(&buffer);
					}
				}
			}
		}
		else {
			this->NormalCopy = false;
		}

		DOMNode* RegexNode = findChildNode(copyer, "Regex");
		if (RegexNode != nullptr) {
			buffer = XMLString::transcode(RegexNode->getAttributes()->getNamedItem(XMLString::transcode("status"))->getTextContent());
			this->RegexCopy = StringToBoolean(buffer);
			XMLString::release(&buffer);
			if (this->RegexCopy) {
				for (DOMNode* node = RegexNode->getFirstChild(); node != nullptr; node = node->getNextSibling()) {
					if (node->getNodeType() == DOMNode::ELEMENT_NODE) {
						buffer = XMLString::transcode(node->getTextContent());
						RegexCopyFilters.push_back(convert.toString(buffer));
						XMLString::release(&buffer);
					}
				}
			}
		}
		else {
			this->RegexCopy = false;
		}

		if (!RegexCopy && !NormalCopy) {
			this->FileCopyer = false;
		}
	}
	XMLPlatformUtils::Terminate();
}

SystemConfig::SystemConfig()
{
	ReadConfig();
}

DOMNode* SystemConfig::findChildNode(DOMNode* node, const char* name)
{
	for (DOMNode* child = node->getFirstChild(); child != 0; child = child->getNextSibling()) {
		if (child->getNodeType() == DOMNode::ELEMENT_NODE && XMLString::compareString(child->getNodeName(), XMLString::transcode(name)) == 0) {
			return child;
		}
	}
	return 0;
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

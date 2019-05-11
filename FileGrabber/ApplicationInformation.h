#pragma once

class ApplicationInformation
{
public:
	static std::wstring getVersion();
	static std::string getANSIVersion();
	static unsigned int getBuild();
private:
	virtual ~ApplicationInformation() = 0;
};

#define FG_VERSION_PTR ApplicationInformation::getVersion().c_str()
#define FG_VERSION ApplicationInformation::getVersion()
#define FG_VERSIONA_PTR ApplicationInformation::getANSIVersion().c_str()
#define FG_VERSIONA ApplicationInformation::getANSIVersion()
#define FG_BUILD ApplicationInformation::getBuild()
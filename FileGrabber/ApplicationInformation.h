#pragma once

class ApplicationInformation
{
public:
	static std::_tstring getVersion();
	static std::string getANSIVersion();
	static unsigned int getBuild();
private:
	virtual ~ApplicationInformation() = 0;
};


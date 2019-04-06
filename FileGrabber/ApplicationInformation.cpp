#include "stdafx.h"
#include "ApplicationInformation.h"
using namespace std;

_tstring ApplicationInformation::getVersion()
{
	return TEXT("0.2.1");
}

std::string ApplicationInformation::getANSIVersion()
{
	return "0.2.1";
}

unsigned int ApplicationInformation::getBuild()
{
	return 9;
}

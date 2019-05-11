#include "stdafx.h"
#include "ApplicationInformation.h"
using namespace std;

wstring ApplicationInformation::getVersion()
{
	return L"0.2.3";
}

string ApplicationInformation::getANSIVersion()
{
	return "0.2.3";
}

unsigned int ApplicationInformation::getBuild()
{
	return 11;
}

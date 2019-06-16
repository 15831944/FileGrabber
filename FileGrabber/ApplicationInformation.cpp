#include "stdafx.h"
#include "ApplicationInformation.h"
using namespace std;

wstring ApplicationInformation::getVersion()
{
	return L"0.4.0";
}

string ApplicationInformation::getANSIVersion()
{
	return "0.4.0";
}

unsigned int ApplicationInformation::getBuild()
{
	return 15;
}

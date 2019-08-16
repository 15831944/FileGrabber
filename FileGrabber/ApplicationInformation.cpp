#include "stdafx.h"
#include "ApplicationInformation.h"
using namespace std;

wstring ApplicationInformation::getVersion()
{
	return L"0.3.2";
}

string ApplicationInformation::getANSIVersion()
{
	return "0.3.2";
}

unsigned int ApplicationInformation::getBuild()
{
	return 16;
}

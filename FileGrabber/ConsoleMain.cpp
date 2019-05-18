#include "stdafx.h"
#include "ApplicationInformation.h"
#include <cstdio>
#include <iostream>
#include "MD5Encrypt.h"
#include <string>
using namespace std;

#pragma warning(disable : 4996)

int ProcessConsoleMain(int argc, wchar_t* argv[]);
void PasswordVerify();

int ConsoleMain(int argc, wchar_t* argv[]) {
	if (argc >= 1 && (wcscmp(argv[0], L"-c") == 0 || wcscmp(argv[0], L"-C") == 0)) {
		return ProcessConsoleMain(argc, argv);
	}
	else {
		MessageBoxW(NULL, L"Invaild param. Type \'-c\' to open FileGrabber console tool.", (L"FileGrabber " + FG_VERSION + L" - Error").c_str(), MB_ICONERROR);
		return -1;
	}
}

int ProcessConsoleMain(int argc, wchar_t* argv[]) {
	AllocConsole();
	SetConsoleTitleW((L"FileGrabber " + FG_VERSION + L" Console Tool").c_str());
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	wcout << L"FileGrabber " << FG_VERSION << L" Console Tool" << endl;
	wcout << L"Copyright (C) ISLStudio, Sty 2014-2019. All rights reserved." << endl << endl;
	PasswordVerify();
	return 0;
}

void PasswordVerify() {
	wstring password;
	MD5Encrypt enc;
	while (true) {
		wcout << L"Enter Password: ";
		getline(wcin, password);
		if (enc.encryptString(password).toString() == L"c09496084c719c4843eca19340fd2f5b") {
			break;
		}
		wcout << L"Error #1: Incorrect password. You must input the correct password to continue." << endl << endl;
	}
}
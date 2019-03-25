#include "stdafx.h"
#include "resource.h"

void PostProgramStartMessage(HWND hWnd) {
	NOTIFYICONDATA nid;
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = hWnd;
	nid.uID = 209;
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP | NIF_INFO;
	nid.uCallbackMessage = WM_USER;
	nid.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_FILEGRABBER));
	lstrcpy(nid.szTip, L"FileGrabber v0.0.3");
	lstrcpy(nid.szInfo, L"FileGrabber v0.0.3 is running...");
	lstrcpy(nid.szInfoTitle, L"FileGrabber Service Started");
	Shell_NotifyIcon(NIM_SETVERSION, &nid);
	Shell_NotifyIcon(NIM_ADD, &nid);
}
#include "stdafx.h"
#include "resource.h"

void PostProgramStartMessage(HWND hWnd, NOTIFYICONDATA &nid) {
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = hWnd;
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP | NIF_INFO;
	nid.uCallbackMessage = WM_NOTIFYMSG;
	nid.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_FILEGRABBER));
	lstrcpy(nid.szTip, L"FileGrabber v0.0.4");
	lstrcpy(nid.szInfo, L"FileGrabber v0.0.4 is running...");
	lstrcpy(nid.szInfoTitle, L"FileGrabber Service Started");
	Shell_NotifyIcon(NIM_SETVERSION, &nid);
	Shell_NotifyIcon(NIM_ADD, &nid);
}
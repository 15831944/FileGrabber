#include "stdafx.h"
#include "NotifyDataManager.h"
using namespace std;

NotifyDataManager::NotifyDataManager(HWND hWnd, UINT CallbackMessage, HICON hIcon, UINT uFlags, std::_tstring Tip, std::_tstring Info, std::_tstring InfoTitle)
{
	_data.cbSize = sizeof(NOTIFYICONDATA);
	_data.hWnd = hWnd;
	_data.uCallbackMessage = CallbackMessage;
	_data.hIcon = hIcon;
	_data.uFlags = uFlags;
	_tcscpy_s(_data.szTip, sizeof(_data.szTip) / sizeof(TCHAR), Tip.c_str());
	_tcscpy_s(_data.szInfo, sizeof(_data.szInfo) / sizeof(TCHAR), Info.c_str());
	_tcscpy_s(_data.szInfoTitle, sizeof(_data.szInfoTitle) / sizeof(TCHAR), InfoTitle.c_str());
	Shell_NotifyIcon(NIM_SETVERSION, &_data);
	Shell_NotifyIcon(NIM_ADD, &_data);
}

void NotifyDataManager::setIcon(HICON hIcon)
{
	_data.hIcon = hIcon;
}

void NotifyDataManager::setTip(const _tstring& Tip)
{
	_tcscpy_s(_data.szTip, sizeof(_data.szTip) / sizeof(TCHAR), Tip.c_str());
}

void NotifyDataManager::setInfo(const _tstring& Info)
{
	_tcscpy_s(_data.szInfo, sizeof(_data.szInfo) / sizeof(TCHAR), Info.c_str());
}

void NotifyDataManager::setInfoTitle(const _tstring& InfoTitle)
{
	_tcscpy_s(_data.szInfoTitle, sizeof(_data.szInfoTitle) / sizeof(TCHAR), InfoTitle.c_str());
}

void NotifyDataManager::updateNotifyWithoutMessaging()
{
	_data.uFlags = NIF_ICON | NIF_TIP | NIF_INFO;
	Shell_NotifyIcon(NIM_MODIFY, &_data);
}

void NotifyDataManager::updateNotify()
{
	_data.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP | NIF_INFO;
	Shell_NotifyIcon(NIM_MODIFY, &_data);
}

void NotifyDataManager::deleteNotify()
{
	Shell_NotifyIcon(NIM_DELETE, &_data);
}

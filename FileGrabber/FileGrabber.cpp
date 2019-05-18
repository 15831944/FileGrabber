// FileGrabber.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "FileGrabber.h"
#include <Dbt.h>
#include "Convert.h"
#include "NotifyDataManager.h"
#include "AESKey.h"
#include "AESEncrypt.h"
#include "ApplicationInformation.h"
#include "Log.h"
using namespace std;

#define MAX_LOADSTRING 100

HINSTANCE hInst;                                
WCHAR szTitle[MAX_LOADSTRING];                  
WCHAR szWindowClass[MAX_LOADSTRING];          
extern bool IsServiceOn;
extern bool IsCopyOn;

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
int ConsoleMain(int argc, wchar_t* argv[]);
BOOL CheckSingleInstance(LPCTSTR pszUniqueName);
void DeviceArrivalMain(TCHAR DriveLetter);
void DeviceRemovalMain(TCHAR DriveLetter);
void InitProgram();

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	if (_tcscmp(lpCmdLine, L"") != 0) {
		int argc;
		wchar_t** argv = CommandLineToArgvW(lpCmdLine, &argc);
		return ConsoleMain(argc, argv);
	}

	if (CheckSingleInstance(L"Global\\82ECF125A20C4C6C91959A89D5F34E36") == FALSE) {
		MessageBox(NULL, L"Only one instance can be launched at a time.", (L"FileGrabber " + FG_VERSION + L" - Error").c_str(), MB_ICONERROR);
		return 0;
	}

	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_FILEGRABBER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FILEGRABBER));

	InitProgram();

	MSG msg;

	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FILEGRABBER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_FILEGRABBER);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance;

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   //ShowWindow(hWnd, nCmdShow);
   //UpdateWindow(hWnd);

   return TRUE;
}

BOOL CheckSingleInstance(LPCTSTR pszUniqueName) {
	HANDLE hMutex = CreateEvent(NULL, TRUE, FALSE, pszUniqueName);
	DWORD dwLstErr = GetLastError();
	BOOL bOneInstanceCheck = TRUE;

	if (hMutex) {
		if (dwLstErr == ERROR_ALREADY_EXISTS) {
			CloseHandle(hMutex);
			bOneInstanceCheck = FALSE;
		}
	}
	else {
		if (dwLstErr == ERROR_ACCESS_DENIED)
			bOneInstanceCheck = FALSE;
	}

	return bOneInstanceCheck;
}

static const GUID GUID_DEVINTERFACE_USB_DEVICE =
{ 0xA5DCBF10, 0x6530, 0x11D2, {0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED} };

void RegisterDeviceNotify(HWND hWnd)
{
	HDEVNOTIFY hDevNotify;
	DEV_BROADCAST_DEVICEINTERFACE NotificationFilter;
	ZeroMemory(&NotificationFilter, sizeof(NotificationFilter));
	NotificationFilter.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
	NotificationFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
	NotificationFilter.dbcc_classguid = GUID_DEVINTERFACE_USB_DEVICE;
	hDevNotify = RegisterDeviceNotification(hWnd, &NotificationFilter, DEVICE_NOTIFY_WINDOW_HANDLE);
}

TCHAR FirstDriveFromMask(ULONG unitmask)
{
	TCHAR i;

	for (i = 0; i < 26; ++i)
	{
		if (unitmask & 0x1)
			break;
		unitmask >>= 1;
	}

	return (i + L'A');
}

LRESULT DeviceChange(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (DBT_DEVICEARRIVAL == wParam || DBT_DEVICEREMOVECOMPLETE == wParam)
	{
		PDEV_BROADCAST_HDR pHdr = (PDEV_BROADCAST_HDR)lParam;
		if (pHdr->dbch_devicetype == DBT_DEVTYP_VOLUME)
		{
			PDEV_BROADCAST_VOLUME pDevVolume = (PDEV_BROADCAST_VOLUME)lParam;
			TCHAR driverLabel = FirstDriveFromMask(pDevVolume->dbcv_unitmask);
			if (wParam == DBT_DEVICEARRIVAL) {
				DeviceArrivalMain(driverLabel);
			}
			else {
				DeviceRemovalMain(driverLabel);
			}
		}
	}
	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DEVICECHANGE:
		return DeviceChange(message, wParam, lParam);
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}
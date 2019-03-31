#pragma once
class NotifyDataManager
{
public:
	NotifyDataManager(HWND hWnd, UINT CallbackMessage, HICON hIcon, UINT uFlags, std::_tstring Tip, std::_tstring Info, std::_tstring InfoTitle);
	void setIcon(HICON hIcon);
	void setTip(const std::_tstring& Tip);
	void setInfo(const std::_tstring& Info);
	void setInfoTitle(const std::_tstring& InfoTitle);
	void updateNotifyWithoutMessaging();
	void updateNotify();
	void deleteNotify();
protected:
	NOTIFYICONDATA _data;
};


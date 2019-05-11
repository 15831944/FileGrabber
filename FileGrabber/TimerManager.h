#pragma once
#include <vector>
#include "Timer.h"

class TimerManager
{
public:
	static TimerManager* getInstance();
	void setWindowHandle(HWND hWnd);
	HWND getWindowHandle() const;
protected:
	TimerManager();
	HWND hWnd;
	std::vector<Timer> managedTimers;
};


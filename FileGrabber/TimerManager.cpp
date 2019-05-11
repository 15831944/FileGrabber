#include "stdafx.h"
#include "TimerManager.h"

namespace {
	TimerManager* instance = nullptr;
}

TimerManager* TimerManager::getInstance()
{
	if (instance == nullptr) {
		instance = new TimerManager();
	}
	return instance;
}

void TimerManager::setWindowHandle(HWND hWnd)
{
	this->hWnd = hWnd;
}

HWND TimerManager::getWindowHandle() const
{
	return this->hWnd;
}

TimerManager::TimerManager()
{
}

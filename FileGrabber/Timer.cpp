#include "stdafx.h"
#include "Timer.h"
#include "TimerManager.h"

uint Timer::CurrentTimerID = 0;

Timer::Timer(ulong time)
{
	this->time = time;
	this->id = obtainNewTimerID();
	this->callback = nullptr;
}

Timer::Timer(ulong time, int id)
{
	this->time = time;
	this->id = id;
	this->callback = nullptr;
}

void Timer::setCallback(TIMERPROC callback)
{
	this->callback = callback;
}

uint Timer::getID() const
{
	return this->id;
}

ulong Timer::getTime() const
{
	return this->time;
}

void Timer::start() const
{
	SetTimer(TimerManager::getInstance()->getWindowHandle(), id, time, callback);
}

void Timer::kill() const
{
	KillTimer(TimerManager::getInstance()->getWindowHandle(), id);
}

uint Timer::obtainNewTimerID()
{
	++CurrentTimerID;
	return CurrentTimerID;
}

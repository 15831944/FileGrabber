#pragma once

class Timer
{
public:
	Timer(ulong time);
	Timer(ulong time, int id);
	void setCallback(TIMERPROC callback);
	uint getID() const;
	ulong getTime() const;
	void start() const;
	void kill() const;
	static uint obtainNewTimerID();
protected:
	ulong time;
	TIMERPROC callback;
	uint id;
	static uint CurrentTimerID;
};


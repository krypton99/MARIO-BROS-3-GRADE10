#pragma once
#include <Windows.h>
#include "Utils.h"
class Timer
{
	DWORD startTime;
	DWORD endTime;
public:
	Timer(int endTime) { this->endTime = endTime; startTime = 0; }

	void Start() { startTime = GetTickCount(); }
	void Reset() { startTime = GetTickCount(); }
	void Stop() { startTime = 0; }

	bool IsTimeUp() {

		return (GetTickCount() - startTime) > endTime;
	}
	DWORD Timeleft() {
		return GetTickCount() - startTime;
	}
	DWORD GetStartTime() { return startTime; }
	DWORD GetEndTime() { return endTime; }
	void SetEndTime(int temp) { endTime = temp; }
};
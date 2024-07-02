#pragma once
#include <time.h>
#include <Windows.h>

class CTimer {
	static CTimer* _instance;

	ULONGLONG currentTime;

public:
	CTimer() {
		currentTime = 0;
	}

	static CTimer* GetInstance() {
		if (!_instance)
			_instance = new CTimer();
		return _instance;
	}

	void InitTime() {
		currentTime = 300000;
	}

	void Update(DWORD dt) {
		currentTime -= dt;
		if (currentTime <= 0)
			currentTime = 0;
	}

	ULONGLONG getCurrentTime() {
		return currentTime;
	}

	int IsTimeOver() {
		return currentTime == 0;
	}
};
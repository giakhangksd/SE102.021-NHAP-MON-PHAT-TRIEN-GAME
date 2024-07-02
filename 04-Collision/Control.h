#pragma once

#include "GameObject.h"

#define CONTROL_TYPE_PAUSE					1
#define CONTROL_TYPE_MODE_ONE				2
#define CONTROL_TYPE_MODE_TWO				3

#define CONTROL_PAUSE_BBOX_WIDTH			71
#define CONTROL_PAUSE_BBOX_HEIGHT			15

#define CONTROL_MODE_BBOX_WIDTH				111
#define CONTROL_MODE_BBOX_HEIGHT			7

class CControl : public CGameObject {
protected:
	static CControl* _instance;

	int control_pause;
	int control_mode_one;
	int control_mode_two;

	int n;
	int active[3];			// Check what control is activating

public:
	CControl() {
		n = 3;
		x = y = 0;
		control_pause = CONTROL_TYPE_PAUSE;
		control_mode_one = CONTROL_TYPE_MODE_ONE;
		control_mode_two = CONTROL_TYPE_MODE_TWO;
		for (int i = 0; i < n; i++)
			active[i] = 0;
	}

	static CControl* GetInstance() {
		if (_instance == NULL)
			_instance = new CControl();
		return _instance;
	}

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void CalculatePosition(int type);
	void Render();

	void ActiveControl(int type) {
		active[type - 1] = 1;
	}

	void DeactiveControl(int type) {
		active[type - 1] = 0;
	}

	int IsPausing() {
		return active[CONTROL_TYPE_PAUSE - 1] == 1;
	}

	int IsActivatingModeOne() {
		return active[CONTROL_TYPE_MODE_ONE - 1] == 1;
	}

	int IsActivatingModeTwo() {
		return active[CONTROL_TYPE_MODE_TWO - 1] == 1;
	}
};


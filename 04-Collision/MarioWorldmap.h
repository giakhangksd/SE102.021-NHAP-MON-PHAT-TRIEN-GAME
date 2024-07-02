#pragma once
#include "Hud.h"
#include "Mario.h"
#include "GameObject.h"

#define MARIO_WORLDMAP_WALKING_SPEED				0.1f

#define MARIO_WORLDMAP_STATE_WALKING_RIGHT			1
#define MARIO_WORLDMAP_STATE_WALKING_LEFT			2
#define MARIO_WORLDMAP_STATE_WALKING_UP				3
#define MARIO_WORLDMAP_STATE_WALKING_DOWN			4

#define ID_ANI_MARIO_SMALL_WORLDMAP					221
#define ID_ANI_MARIO_BIG_WORLDMAP					821
#define ID_ANI_MARIO_RACOON_WORLDMAP				1531

#define MARIO_BBOX_WIDTH							7
#define MARIO_BBOX_HEIGHT							15

#define MARIO_CROSSING_RANGE						15

#define MARIO_CROSSING_TIME							200

class CMarioWorldmap : public CGameObject {
protected:
	int ny;
	int level;
	int canGoIntoPortal;
	int absolutelyTouching;

	float old_pos;

	ULONGLONG crossingStart;

public:
	CMarioWorldmap(float x, float y) : CGameObject(x, y) {
		nx = 1;
		ny = 0;
		vx = vy = 0;
		old_pos = 0;
		crossingStart = 0;
		canGoIntoPortal = 0;
		absolutelyTouching = 0;
		level = CHud::GetInstance()->GetLevel();
	}

	void SetState(int State);
	void SetCanGoIntoPortal(int value) {
		canGoIntoPortal = value;
	}

	int IsBlocking() {
		return 1;
	}

	int IsCollidable() {
		return 1;
	}

	int IsMoving() {
		if (vx || vy || crossingStart || absolutelyTouching) return 1;
		return 0;
	}

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPGAMEOBJECT o);
	void OnCollisionWith(LPCOLLISIONEVENT e);

	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
};
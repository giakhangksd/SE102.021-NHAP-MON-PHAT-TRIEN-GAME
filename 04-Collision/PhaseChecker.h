#pragma once

#include "GameObject.h"

#define PHASE_CHECKER_GRAVITY				0.001f

#define PHASECHECK_ATTACK_TIME				300

class CPhaseChecker : public CGameObject {
protected:
	int width;
	int height;

	float ax;
	float ay;

	virtual int IsCollidable() { return 1; }
	virtual int IsBlocking() { return 0; }

	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	virtual void Render();

public:
	CPhaseChecker(float x, float y, int w, int h) : CGameObject(x, y) {
		ax = vy = 0;
		ay = PHASE_CHECKER_GRAVITY;

		width = w;
		height = h;
	}
};
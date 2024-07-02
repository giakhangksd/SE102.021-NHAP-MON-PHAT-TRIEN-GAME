#pragma once

#include "GameObject.h"

#define BULLET_SPEED_X				0.02f
#define BULLET_SPEED_Y				0.02f

#define BULLET_DIRECTION_LEFT		-1.0f
#define BULLET_DIRECTION_TOP		-1.0f
#define BULLET_DIRECTION_RIGHT		1.0f
#define BULLET_DIRECTION_BOTTOM		1.0f

#define BULLET_BBOX_WIDTH			7
#define BULLET_BBOX_HEIGHT			8

#define ID_ANI_BULLET_FIRE			8001

#define BULLET_BY_PIRANHA			1
#define BULLET_BY_MARIO				2

class CBullet :public CGameObject {
protected:
	int type;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Render();

	virtual int IsCollidable() { return 1; }
	virtual int IsBlocking() { return 0; }

	virtual void OnNoCollision(DWORD dt) {}
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
public:
	CBullet(float x, float y, int type) : CGameObject(x, y) {
		this->type = type;
		vx = vy = 0;
	}

	void SetDirection(float d1, float d2) {
		vx = d1 * BULLET_SPEED_X;
		vy = d2 * BULLET_SPEED_Y;
	}

	int GetType() {
		return type;
	}
};


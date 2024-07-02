#pragma once
#include "GameObject.h"

#define ENEMY_WALKING_SPEED				0.01f

#define ENEMY_WALKING_RIGHT				20002
#define ENEMY_WALKING_LEFT				20003

#define ENEMY_BBOX_WIDTH				15
#define ENEMY_BBOX_HEIGHT				15

class CEnemyWorldmap : public CGameObject {
protected:
	float old_x;
	float walkingRange;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Render();

	virtual int IsCollidable() {
		return 0;
	}


	virtual int IsBlocking() {
		return 0;
	}

public:
	CEnemyWorldmap(float x, float y, float range = 0) : CGameObject(x, y) {
		old_x = x;
		walkingRange = range;

		vx = ENEMY_WALKING_SPEED;
		vy = 0;

		nx = 1;
	}

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
};
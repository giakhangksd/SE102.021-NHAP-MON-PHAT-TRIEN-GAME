#pragma once
#include "GameObject.h"

#define GOOMBA_GRAVITY 0.0004f
#define GOOMBA_WALKING_SPEED 0.04f


#define GOOMBA_BBOX_WIDTH 16
#define GOOMBA_BBOX_HEIGHT 14
#define GOOMBA_BBOX_HEIGHT_DIE 7

#define GOOMBA_DIE_TIMEOUT 500

#define GOOMBA_STATE_WALKING 100
#define GOOMBA_STATE_DIE 200

#define GOOMBA_STATE_DIE_SHELL 230

#define GOOMBA_STATE_WING_FLY 400
#define GOOMBA_STATE_WING_WALK_RIGHT 500
#define GOOMBA_STATE_WING_WALK 600

#define ID_ANI_GOOMBA_WALKING 5000
#define ID_ANI_GOOMBA_DIE 5001
#define ID_ANI_GOOMBA_WING_WALK 5002
#define ID_ANI_GOOMBA_WING_FLY 5003


class CGoomba : public CGameObject
{
protected:
	float ax;
	float ay;

	int hit_nx;
	ULONGLONG die_start, wait_2_fly, wait_2_walk;
	BOOLEAN isFellDown, isOnPlatform;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual void OnNoCollision(DWORD dt);
	virtual int IsBlocking() { return 0; };

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public: 	
	CGoomba(float x, float y,int type);
	virtual void SetState(int state);
};
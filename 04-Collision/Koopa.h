#pragma once
#include "GameObject.h"

#define KOOPA_GRAVITY 0.0005f
#define KOOPA_WALKING_SPEED 0.05f


#define KOOPA_BBOX_WIDTH 16
#define KOOPA_BBOX_HEIGHT 14
#define KOOPA_BBOX_HEIGHT_DIE 7

#define KOOPA_WAIT_TIMEOUT 500

#define KOOPA_STATE_WALKING 1000
#define KOOPA_STATE_SHELL 2000
#define KOOPA_STATE_SHELL_RIGHT 2200
#define KOOPA_STATE_SHELL_MOV 3000
#define KOOPA_STATE_WING_FLY 4000
#define KOOPA_STATE_SHELL_MOV_RIGHT 5000
#define KOOPA_STATE_SHELL_CHANGE 6000
#define KOOPA_STATE_WALKING_RIGHT 7000
#define KOOPA_STATE_SHELL_CHANGE_RIGHT 8000

#define ID_ANI_KOOPA_WALKING 6000
#define ID_ANI_KOOPA_SHELL 6001
#define ID_ANI_KOOPA_WING_FLY 6002
#define ID_ANI_KOOPA_WALKING_RIGHT 6003
#define ID_ANI_KOOPA_SHELL_CHANGE 6004
#define ID_ANI_KOOPA_SHELL_CHANGE_RIGHT 6005


class CKoopa : public CGameObject
{
protected:
	float ax;
	float ay;
	int nx;

	BOOLEAN isBlockByPlatform;
	BOOLEAN isOnPlatform;

	float l_bounded, r_bounded;
	BOOLEAN isOnBlock;

	ULONGLONG die_start, wait1, wait2, wait3;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	virtual void OnCollisionWithPlatform(LPCOLLISIONEVENT e);
	virtual void OnCollisionWithOthers(LPCOLLISIONEVENT e);

public:
	CKoopa(float x, float y, int type);
	virtual void SetState(int state);
};
#pragma once

#ifndef CKOOPAS_H
#define CKOOPAS_H

#include "PhaseChecker.h"
#include "GameObject.h"

#define KOOPA_TROOPA_GRAVITY						0.001f
#define KOOPA_TROOPA_WALKING_SPEED					0.04f
#define KOOPA_TROOPA_SHELL_SPEED					0.2f

#define KOOPA_TROOPA_FLY_SPEED_Y					0.2f

#define KOOPA_TROOPA_DIE_DEFLECT					0.3f

#define KOOPA_TROOPA_BBOX_WIDTH						14
#define KOOPA_TROOPA_BBOX_HEIGHT					24
#define KOOPA_TROOPA_BBOX_HEIGHT_DIE				15

#define KOOPA_TROOPA_PHASE_CHECK_WIDTH				14
#define KOOPA_TROOPA_PHASE_CHECK_HEIGHT				24

#define KOOPA_TROOPA_SHELL_TIMEOUT					5000
#define KOOPA_TROOPA_DIE_TIMEOUT					500
#define KOOPA_TROOPA_FLY_TIMEOUT					300

#define KOOPA_TROOPA_STATE_FLYING					50
#define KOOPA_TROOPA_STATE_WALKING					100
#define KOOPA_TROOPA_STATE_SHELL					200
#define KOOPA_TROOPA_STATE_ATTACKING				300
#define KOOPA_TROOPA_STATE_DIE						400

#define KOOPA_TROOPA_LEVEL_NORMAL					1
#define KOOPA_TROOPA_LEVEL_PARA						2

#define KOOPA_TROOPA_TYPE_RED						1
#define KOOPA_TROOPA_TYPE_GREEN						2

#define ID_ANI_RED_KOOPA_TROOPA_WALKING_LEFT		6011
#define ID_ANI_RED_KOOPA_TROOPA_WALKING_RIGHT		6012
#define ID_ANI_RED_KOOPA_TROOPA_SHELL_DOWN			6021
#define ID_ANI_RED_KOOPA_TROOPA_SHELL_UP			6022
#define ID_ANI_RED_KOOPA_TROOPA_ATTACKING_DOWN		6031
#define ID_ANI_RED_KOOPA_TROOPA_ATTACKING_UP		6032
#define ID_ANI_RED_KOOPA_TROOPA_REFORM_DOWN			6041
#define ID_ANI_RED_KOOPA_TROOPA_REFORM_UP			6042

#define ID_ANI_GREEN_KOOPA_TROOPA_WALKING_LEFT		6111
#define ID_ANI_GREEN_KOOPA_TROOPA_WALKING_RIGHT		6112
#define ID_ANI_GREEN_KOOPA_TROOPA_SHELL_DOWN		6121
#define ID_ANI_GREEN_KOOPA_TROOPA_SHELL_UP			6122
#define ID_ANI_GREEN_KOOPA_TROOPA_ATTACKING_DOWN	6131
#define ID_ANI_GREEN_KOOPA_TROOPA_ATTACKING_UP		6132
#define ID_ANI_GREEN_KOOPA_TROOPA_REFORM_DOWN		6141
#define ID_ANI_GREEN_KOOPA_TROOPA_REFORM_UP			6142

#define ID_ANI_GREEN_PARA_KOOPA_TROOPA_FLYING_LEFT	6211
#define ID_ANI_GREEN_PARA_KOOPA_TROOPA_FLYING_RIGHT 6212

class CKoopaTroopa : public CGameObject
{
protected:
	float ax;
	float ay;
	
	int type;
	int level;

	bool isUp;
	bool isHeld;

	ULONGLONG time_start;
	ULONGLONG deflected_start;

	CGameObject* phaseCheck;

	virtual int IsCollidable() { return state != KOOPA_TROOPA_STATE_DIE && !isHeld; };
	virtual int IsBlocking() { return 0; }
	virtual void Deflected(int direction);

	virtual void OnCollisionWithBrick(LPCOLLISIONEVENT e);
	virtual void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	virtual void OnCollisionWithKoopaTroopa(LPCOLLISIONEVENT e);
	virtual void OnCollisionWithPiranhaPlant(LPCOLLISIONEVENT e);

	int GetAniId();

public:
	CKoopaTroopa(float x, float y, int type,int level);
	virtual void SetState(int state);
	//void HoldByMario(float* x, float* y, int* nx);
	//void UpdatePositionFollowMario();
	void SetNx(int nx) {
		this->nx = nx;
	}

	int GetNx() {
		return nx;
	}

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();

	int GetStateHeld() { return isHeld; }
	void IsHeld() { isHeld = true; }

	int GetLevel() {
		return level;
	}

	void SetLevel(int l) {
		level = l;
	}
};

#endif
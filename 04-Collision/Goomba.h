#pragma once
#include "GameObject.h"

#define GOOMBA_GRAVITY 0.0004f
#define GOOMBA_WALKING_SPEED 0.04f

#define GOOMBA_JUMP_SPEED_Y	0.08f
#define GOOMBA_FLY_SPEED_Y	0.3f
#define GOOMBA_PARA_BBOX_HEIGHT	20
#define GOOMBA_DIE_DEFLECT	0.2f

#define GOOMBA_BBOX_WIDTH 16
#define GOOMBA_BBOX_HEIGHT 14
#define GOOMBA_BBOX_HEIGHT_DIE 7

#define GOOMBA_DIE_TIMEOUT 500

#define GOOMBA_STATE_WALKING 100
#define GOOMBA_STATE_JUMPING	50
#define GOOMBA_STATE_FLYING	55
#define GOOMBA_STATE_DIE_BY_JUMP	200
#define GOOMBA_STATE_DIE_BY_ATTACK	300

#define ID_ANI_GOOMBA_WALKING 5000
#define ID_ANI_GOOMBA_DIE_BY_JUMP	5001
#define ID_ANI_GOOMBA_DIE_BY_ATTACK	5002

#define ID_ANI_RED_GOOMBA_WALKING			5100
#define ID_ANI_RED_GOOMBA_DIE_BY_JUMP		5101
#define ID_ANI_RED_GOOMBA_DIE_BY_ATTACK		5102

#define ID_ANI_RED_PARA_GOOMBA_FLYING		5200
#define ID_ANI_RED_PARA_GOOMBA_WALKING		5201
#define ID_ANI_RED_PARA_GOOMBA_JUMPING		5202

#define GOOMBA_TYPE_NORMAL					1
#define GOOMBA_TYPE_RED						2

#define GOOMBA_LEVEL_NORMAL					1
#define GOOMBA_LEVEL_PARA					2

#define GOOMBA_JUMP							3

#define GOOMBA_RED_PARA_WALK_TIME			1000
#define GOOMBA_RED_PARA_RELEASE_JUMP_TIME	100


class CGoomba : public CGameObject
{
protected:
	float ax;
	float ay;

	int type;
	int level;
	int jump_count;

	ULONGLONG time_start;
	BOOLEAN redpara_start;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	virtual void Render();

	virtual int IsCollidable() { return state != GOOMBA_STATE_DIE_BY_JUMP && state != GOOMBA_STATE_DIE_BY_ATTACK; };
	virtual void OnNoCollision(DWORD dt);
	virtual int IsBlocking() { return 0; };

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);


	void OnCollisionWithBrick(LPCOLLISIONEVENT e);
	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithKoopaTroopa(LPCOLLISIONEVENT e);
	int getAniId();

public: 	
	CGoomba(float x, float y, int type, int l = 1);

	virtual void SetState(int state);
	virtual void Deflected(int direction);

	int GetType() {
		return type;
	}

	void SetType(int type) {
		this->type = type;
	}

	void SetLevel(int l);
	int GetLevel() {
		return level;
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
};
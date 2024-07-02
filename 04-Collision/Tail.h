#pragma once

#include "Brick.h"
#include "Goomba.h"
#include "GameObject.h"
#include "PiranhaPlant.h"

#define TAIL_GRAVITY				0.001f
#define TAIL_ATTACK_SPEED			1.5f
#define TAIL_ATTACK_RANGE			3.0f

#define TAIL_COLLIDED_BRICK			1
#define TAIL_COLLIDED_GOOMBA		2
#define TAIL_COLLIDED_KOOPAS		3
#define TAIL_COLLIDED_PLANT			4

#define TAIL_BBOX_WIDTH				15
#define TAIL_BBOX_HEIGHT			5

#define TAIL_ATTACK_TIME			300

class CTail : public CGameObject {
protected:
	float ay;
	float old_x;

	int isAttackedFront;
	int isAttackedBehind;

	ULONGLONG attack_start;

	virtual int IsCollidable() { return 1; }
	virtual int IsBlocking() { return 0; }

	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	virtual void OnCollisionWith(LPGAMEOBJECT o);

	void OnCollisionWithBrick(LPCOLLISIONEVENT e);
	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithKoopas(LPCOLLISIONEVENT e);
	void OnCollisionWithPlant(LPCOLLISIONEVENT e);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

public:
	CTail(float x, float y) :CGameObject(x, y) {
		this->ay = TAIL_GRAVITY;
		isAttackedFront = 0;
		isAttackedBehind = 0;
		attack_start = 0;
		old_x = 0;
	}

	void Attack(int direction) {
		vx += TAIL_ATTACK_SPEED * direction;
		old_x = x;
		isAttackedFront = 0; // only attack 1 gameobject in front of Mario
		isAttackedBehind = 0;  // only attack 1 gameobject(except Block, Platform) behide Mario

		nx = direction;
	}

	bool IsAttacking() { 
		return attack_start != 0; 
	}

	void SetAttackTime(ULONGLONG time) {
		attack_start = time;
	}
};

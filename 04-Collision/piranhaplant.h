#pragma once
#include "GameObject.h"
#include "Bullet.h"

#define PIRANHA_UP_DOWN_SPEED						0.02f

#define PIRANHA_TYPE_GREEN							1
#define PIRANHA_TYPE_GREEN_FIRE						2
#define PIRANHA_TYPE_RED_FIRE						3

#define PIRANHA_STATE_UP							111
#define PIRANHA_STATE_DOWN							222
#define PIRANHA_STATE_ATTACK						333
#define PIRANHA_STATE_IDLE							444

#define PIRANHA_GREEN_BBOX_WIDTH					15
#define PIRANHA_GREEN_BBOX_HEIGHT					23

#define PIRANHA_RED_BBOX_WIDTH						15
#define PIRANHA_RED_BBOX_HEIGHT						31

#define DISTANCE_APPEAR								30

#define ID_ANI_PIRANHA_GREEN						7000

#define ID_ANI_PIRANHA_GREEN_FIRE_LEFT				7001
#define ID_ANI_PIRANHA_GREEN_TARGET_TOP_LEFT		7041
#define ID_ANI_PIRANHA_GREEN_TARGET_BOTTOM_LEFT		7021

#define ID_ANI_PIRANHA_GREEN_FIRE_RIGHT				7011
#define ID_ANI_PIRANHA_GREEN_TARGET_TOP_RIGHT		7051
#define ID_ANI_PIRANHA_GREEN_TARGET_BOTTOM_RIGHT	7031

#define ID_ANI_PIRANHA_RED_FIRE_LEFT				7002
#define ID_ANI_PIRANHA_RED_TARGET_TOP_LEFT			7042
#define ID_ANI_PIRANHA_RED_TARGET_BOTTOM_LEFT		7022

#define ID_ANI_PIRANHA_RED_FIRE_RIGHT				7012
#define ID_ANI_PIRANHA_RED_TARGET_TOP_RIGHT			7052
#define ID_ANI_PIRANHA_RED_TARGET_BOTTOM_RIGHT		7032

#define PIRANHA_ATTACK_IDLE_TIME					5000

class CPiranhaPlant : public CGameObject
{
protected:
	int ay;
	int type;

	float old_y;
	float mario_x, mario_y;

	ULONGLONG time_start;
	CGameObject* _bullet;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt){};

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

	int GetAniId();
public:
	CPiranhaPlant(float x, float y, int type) :CGameObject(x, y) {
		this->type = type;
		old_y = y;
		_bullet = NULL;
		time_start = ay = 0;
		mario_x = mario_y = vy = vx = 0.0;
		SetState(PIRANHA_STATE_UP);
	}
	virtual void SetState(int state);

};


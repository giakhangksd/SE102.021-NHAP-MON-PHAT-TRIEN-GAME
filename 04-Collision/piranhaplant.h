#pragma once
#include "GameObject.h"

#define	PLANT_WIDTH 16
#define PLANT_BBOX_WIDTH 10
#define PLANT_BBOX_HEIGHT 26

#define PLANT_STATE_UP_BITE 355
#define PLANT_STATE_DOWN_BITE 365
#define PLANT_STATE_READY_FIRE 375
#define PLANT_STATE_FIRE 385

#define ID_ANI_PLANT_BITE 5650
#define ID_ANI_PLANT_LEFT 5660
#define ID_ANI_PLANT_RIGHT 5670
#define ID_ANI_PLANT_FIRE 5680

class CPlant : public CGameObject
{
protected:


	ULONGLONG wait1;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 0; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);
	

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CPlant(float x, float y);
	virtual void SetState(int state);

};


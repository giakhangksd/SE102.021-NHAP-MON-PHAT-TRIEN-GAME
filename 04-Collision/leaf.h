#pragma once
#pragma once
#include "GameObject.h"

#define LEAF_GRAVITY 0.0001f
#define LEAF_FALLING_L -0.005f
#define LEAF_FALLING_R 0.005f

#define	LEAF_WIDTH 16
#define LEAF_BBOX_WIDTH 16
#define LEAF_BBOX_HEIGHT 16

#define LEAF_STATE_FALLING 450
#define LEAF_STATE_WAITING 460
#define ID_ANI_LEAF_FALLING 5800

class CLeaf : public CGameObject
{
protected:
	int a;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 0; };

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CLeaf(float x, float y);
	virtual void SetState(int state);
};

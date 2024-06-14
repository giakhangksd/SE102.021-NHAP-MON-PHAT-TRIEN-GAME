#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_QUESBOX 12000
#define ID_ANI_QUESBOX_NOT 12001
#define QUESBOX_STATE 150
#define QUESBOX_STATE_NOT 250


#define QUESBOX_WIDTH 16
#define QUESBOX_BBOX_WIDTH 16
#define QUESBOX_BBOX_HEIGHT 16

class CQuesbox : public CGameObject {
public:
	float bef_x,bef_y;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsBlocking() { return 1; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

	CQuesbox(float x, float y);
	virtual void SetState(int state);
};
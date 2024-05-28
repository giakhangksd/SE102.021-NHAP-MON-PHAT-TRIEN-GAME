#pragma once


#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_COINJUMP 13000

#define	COINJUMP_WIDTH 16
#define COINJUMP_BBOX_WIDTH 16
#define COINJUMP_BBOX_HEIGHT 16

#define COIN_STATE_WAITING 375
#define COIN_STATE_ACTIVE 385

#define COIN_DIE_TIMEOUT 555

class CCoinjump : public CGameObject {
public:
	float ay;
	float ax;

	ULONGLONG die_start;

	CCoinjump(float x, float y);
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual int IsBlocking() { return 0; }
	virtual void SetState(int state);

	virtual void OnNoCollision(DWORD dt);


};
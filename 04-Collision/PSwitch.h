#pragma once

#include "GameObject.h"

#define ID_ANI_PSWITCH						8010 
#define ID_ANI_PSWITCH_ACTIVED				8011 // after collide with Mario

#define PSWITCH_BBOX_WIDTH					15
#define PSWITCH_BBOX_HEIGHT					15

#define PSWITCH_ACTIVED_BBOX_WIDTH			0
#define PSWITCH_ACTIVED_BBOX_HEIGHT			0

class CPSwitch : public CGameObject
{
protected:
	bool isActived;

public:
	CPSwitch(float x, float y) : CGameObject(x, y) { isActived = false; }
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
	int IsCollidable() { return 0; }
	void IsActived();
};
#pragma once


#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_COINJUMP 12000

#define	COINJUMP_WIDTH 16
#define COINJUMP_BBOX_WIDTH 16
#define COINJUMP_BBOX_HEIGHT 16

class CCoinjump : public CGameObject {
public:
	CCoinjump(float x, float y) : CGameObject(x, y) {}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
};
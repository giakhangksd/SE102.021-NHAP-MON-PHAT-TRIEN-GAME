#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANY_UNDERPIPE 25000
#define UNDERPIPE_WIDTH 16
#define UNDERPIPE_BBOX_WIDTH 32
#define UNDERPIPE_BBOX_HEIGHT 32

class Cunderpipe : public CGameObject {
public:
	Cunderpipe(float x, float y) : CGameObject(x, y) {}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

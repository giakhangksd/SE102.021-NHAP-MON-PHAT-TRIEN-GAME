#pragma once
#include "GameObject.h"

#define PLATFORM_ANIMATE_BBOX_WIDTH				15
#define PLATFORM_ANIMATE_BBOX_HEIGHT			15

#define PLATFORM_ANIMATE_TYPE_BLOCK				1
#define PLATFORM_ANIMATE_TYPE_GATE				2
#define PLATFORM_ANIMATE_TYPE_NO_COLLISION		3

class CPlatformAnimate : public CGameObject {
protected:
	int type;
	int isAni; // 1: animation, 0: sprite
	int aniOrsprite;

	virtual int IsBlocking() {
		return type != PLATFORM_ANIMATE_TYPE_NO_COLLISION;
	}

public:
	CPlatformAnimate(float x, float y, int AniOrSprite, int Type = 1, int IsAni = 1) : CGameObject(x, y) {
		type = Type;
		isAni = IsAni;
		aniOrsprite = AniOrSprite;
	}

	int GetType() {
		return type;
	}

	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};
#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_COIN 11000
#define ID_ANI_COIN_DEFLECT				11001

#define COIN_DEFLECT					0.38f
#define COIN_GRAVITY					0.001f

#define COIN_BBOX_WIDTH					15
#define COIN_BBOX_HEIGHT				15

#define COIN_BY_NONE					0
#define COIN_TRANSFORMED_FROM_BRICK		1

#define COIN_TIMEOUT					10000
//#define	COIN_WIDTH 10
//#define COIN_BBOX_WIDTH 10
//#define COIN_BBOX_HEIGHT 16

class CCoin : public CGameObject {
private:
	float ay;
	float old_y;

	int type;

	ULONGLONG remain_start;

	void CoinTransformBrick();
public:
	CCoin(float x, float y, int type = 0) : CGameObject(x, y) {
		ay = vy = 0.0;
		remain_start = 0;
		this->type = type;
		if (type == COIN_TRANSFORMED_FROM_BRICK) remain_start = GetTickCount64();
	}
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
	void Deflected(int direction);
};
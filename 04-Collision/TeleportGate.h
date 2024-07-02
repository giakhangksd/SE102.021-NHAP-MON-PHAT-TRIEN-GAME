#pragma once
#include "GameObject.h"

#define TELEPORT_SPEED						0.02f

#define TELEPORT_BBOX_WIDTH					15
#define TELEPORT_BBOX_HEIGHT				15

#define TELEPORT_DIRECTION_DOWN				0
#define TELEPORT_DIRECTION_UP				1

#define TELEPORT_TIME						1000

class CTeleportGate : public CGameObject {
protected:
	float des_x; 
	float des_y;

	int direction;

	ULONGLONG tele_start;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Render();

	virtual int IsCollidable() { return 1; }
	virtual int IsBlocking() { return 0; }

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void OnCollisionWith(LPGAMEOBJECT obj);

public:
	CTeleportGate(float x, float y, float destination_x, float destination_y, int direction = 0) : CGameObject(x, y) {
		des_x = destination_x;
		des_y = destination_y;
		tele_start = 0;
		this->direction = direction;
	}

	void TeleObject(LPGAMEOBJECT obj);
};


#pragma once

/*****************************
Using this in case: when I should update Koopa Para Troopa
****************************/

#include "GameObject.h"
#include "debug.h"

#define INVISIBLE_BBOX_WIDTH				10
#define INVISIBLE_BBOX_HEIGHT				500

class CInvisibleObject : public CGameObject {
	bool isActived;

public:
	CInvisibleObject(float x, float y) : CGameObject(x, y) {
		isActived = false;
	}

	void Render() { 
		//RenderBoundingBox(); 
	}

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);

	int IsBlocking() { 
		return 0;
	}

	void Activating() { 
		isActived = true; 
	}

	bool IsActived() {
		//DebugOut(L"isActived: %d\n", isActived); 
		return isActived; 
	}
};

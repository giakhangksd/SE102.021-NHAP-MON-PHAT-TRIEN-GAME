#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_QUESBOX 12000
#define ID_ANI_QUESBOX_NOT 12001

#define QUESBOX_WIDTH 16
#define QUESBOX_BBOX_WIDTH 16
#define QUESBOX_BBOX_HEIGHT 16

class CQuesbox : public CGameObject {
public:
	CQuesbox(float x, float y) : CGameObject(x, y) {}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	//virtual void SetState(int state);
	
};
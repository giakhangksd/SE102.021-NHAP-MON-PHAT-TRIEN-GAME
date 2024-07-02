#include "InvisibleObject.h"

void CInvisibleObject::GetBoundingBox(float& l, float& t, float& r, float& b) {
	l = x - INVISIBLE_BBOX_WIDTH / 2;
	t = y - INVISIBLE_BBOX_HEIGHT / 2;
	r = l + INVISIBLE_BBOX_WIDTH;
	b = t + INVISIBLE_BBOX_HEIGHT;
}

void CInvisibleObject::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {

}
#include "leaf.h"

CLeaf::CLeaf(float x, float y) :CGameObject(x, y) 
{
	this->ax = 0;
	this->ay = LEAF_GRAVITY;

	SetState(LEAF_STATE_FALLING);
}

void CLeaf::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - LEAF_BBOX_WIDTH / 2;
	t = y - LEAF_BBOX_HEIGHT / 2;
	r = l +	LEAF_BBOX_WIDTH;
	b = t + LEAF_BBOX_HEIGHT;
}

void CLeaf::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};
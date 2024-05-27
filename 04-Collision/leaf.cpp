#include "leaf.h"

CLeaf::CLeaf(float x, float y) :CGameObject(x, y) 
{
	this->ax = 0;
	this->ay = LEAF_GRAVITY;

	SetState(LEAF_STATE_FALLING);
}

void CLeaf::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - LEAF_BBOX_WIDTH ;
	t = y - LEAF_BBOX_HEIGHT ;
	r = l +	LEAF_BBOX_WIDTH;
	b = t + LEAF_BBOX_HEIGHT;
}

void CLeaf::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CLeaf::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CLeaf*>(e->obj)) return;


}

void CLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CLeaf::Render()
{
	int aniId = ID_ANI_LEAF_FALLING;

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CLeaf::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case LEAF_STATE_FALLING:
		vy = LEAF_GRAVITY / 2 ;
		break;
	}
}
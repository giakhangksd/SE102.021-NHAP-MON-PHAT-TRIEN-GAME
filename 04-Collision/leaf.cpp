#include "leaf.h"

CLeaf::CLeaf(float x, float y) :CGameObject(x, y) 
{
	this->a = 1;
	SetState(LEAF_STATE_WAITING);
}

void CLeaf::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - LEAF_BBOX_WIDTH / 2;
	t = y - LEAF_BBOX_HEIGHT / 2 ; 
	r = l +	LEAF_BBOX_WIDTH;
	b = t + LEAF_BBOX_HEIGHT; 
}

void CLeaf::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CLeaf*>(e->obj)) return;

}

void CLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	y += vy * dt;
	x += vx * dt;
	if (state == LEAF_STATE_FALLING) {
		if (vy <= 0) {
			vy += 0.0008f * dt;
		}
		if (vy > 0) {
			vy = 0.0008f * dt;
			if (a == -1) {
				if (vx >= -0.05f)
					vx -= 0.0001f * dt;
				else
				{
					a = 1;
				}
			}
			else
				if (a == 1) {
					if (vx <= 0.05f)
						vx += 0.0001f * dt;
					else
					{
						a = -1;
					}
				}
		}
	}

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
	case LEAF_STATE_WAITING:
		break;
	case LEAF_STATE_FALLING:
		vy = -0.18f;
		break;
	}
}
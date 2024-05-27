#include "Mushroom.h"

CMushroom::CMushroom(float x, float y) :CGameObject(x, y)
{
	this->ax = 0;
	this->ay = 0;

	SetState(MUSHROOM_STATE_WAITING);
}

void CMushroom::GetBoundingBox(float& l, float& t, float& r, float& b)
{
		l = x - MUSHROOM_BBOX_WIDTH / 2;
		t = y - MUSHROOM_BBOX_HEIGHT / 2;
		r = l + MUSHROOM_BBOX_WIDTH;
		b = t + MUSHROOM_BBOX_HEIGHT;

}

void CMushroom::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CMushroom::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CMushroom*>(e->obj)) return;

	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
	}
	else if (e->nx != 0 && e->obj->IsBlocking())
	{
		vx = -vx;
	}

}

void CMushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;


	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CMushroom::Render()
{
	int aniId = ID_ANI_MUSHROOM_WALKING;

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CMushroom::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case MUSHROOM_STATE_WAITING:
		break;
	case MUSHROOM_STATE_WALKING:

		vy = -0.2f;
		vx = -MUSHROOM_WALKING_SPEED;
		ay =  MUSHROOM_GRAVITY;
		break;
	}
}
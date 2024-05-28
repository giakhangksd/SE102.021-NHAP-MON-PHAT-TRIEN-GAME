#include "Coineffect.h"

CCoinjump::CCoinjump(float x, float y) :CGameObject(x, y)
{
	this->ay = 0;
	this->ax = 0;
	die_start = -1;
	SetState(COIN_STATE_WAITING);
}

void CCoinjump::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - COINJUMP_BBOX_WIDTH / 2;
	t = y - COINJUMP_BBOX_HEIGHT / 2;
	r = l + COINJUMP_BBOX_WIDTH;
	b = t + COINJUMP_BBOX_HEIGHT;
}

void CCoinjump::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CCoinjump::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	if ((state == COIN_STATE_ACTIVE) && (GetTickCount64() - die_start > COIN_DIE_TIMEOUT))
	{
		isDeleted = true;
		return;
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CCoinjump::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_COINJUMP)->Render(x, y);

	//RenderBoundingBox();
}

void CCoinjump::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case COIN_STATE_WAITING:

		break;
	case COIN_STATE_ACTIVE:
		die_start = GetTickCount64();
		vy = -0.2f;
		ay = 0.0005f;
		break;
	}
}
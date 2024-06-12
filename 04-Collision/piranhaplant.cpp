#include "piranhaplant.h"
#include "Mario.h"

CPlant::CPlant(float x, float y) :CGameObject(x, y)
{

	wait1 = -1;
	SetState(PLANT_STATE_UP_BITE);
}

void CPlant::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - PLANT_BBOX_WIDTH / 2;
	t = y - PLANT_BBOX_HEIGHT / 2;
	r = l + PLANT_BBOX_WIDTH;
	b = t + PLANT_BBOX_HEIGHT;

}

void CPlant::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CPlant::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CPlant*>(e->obj)) return;

	if (e->ny != 0 )
	{
		vy = 0;
	}
	else if (e->nx != 0 )
	{
		vx = -vx;
	}
}

void CPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	if (state ==PLANT_STATE_DOWN_BITE && (GetTickCount64() - wait1 > 500 * 5)) {
		SetState(PLANT_STATE_UP_BITE);
	}
	if (state == PLANT_STATE_UP_BITE && (GetTickCount64() - wait1 > 500 * 5)) {
		SetState(PLANT_STATE_DOWN_BITE);
	}
	if (state == PLANT_STATE_UP_BITE && y > 170) {
		vy = 0;
	}
	if (state == PLANT_STATE_DOWN_BITE && y < 129) {
		vy = 0;
	}


	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CPlant::Render()
{
	int aniId = ID_ANI_PLANT_BITE;
	if (state == PLANT_STATE_READY_FIRE) {
		aniId = ID_ANI_PLANT_RIGHT;
		//aniId = ID_ANI_PLANT_LEFT;
	}

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CPlant::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case PLANT_STATE_DOWN_BITE:
		y = 170 ;
		vy = -0.05;
		wait1 = GetTickCount64();
		break;
	case PLANT_STATE_UP_BITE:
		y = 129;
		vy = 0.05;
		wait1 = GetTickCount64();
		break;
	case PLANT_STATE_READY_FIRE:

		break;
	}
}
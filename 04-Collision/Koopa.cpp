#include "Koopa.h"
int  b = 0, c = 0;
CKoopa::CKoopa(float x, float y, int type) :CGameObject(x, y)
{
	this->ax = 0;
	this->ay = KOOPA_GRAVITY;
	die_start = -1;
	wait1 = wait2 = wait3 = -1;
	if (type == 0) {
		SetState(KOOPA_STATE_WALKING);
	}
	if (type == 1) {
		SetState(KOOPA_STATE_WING_FLY);
	}
}

void CKoopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
		left = x - KOOPA_BBOX_WIDTH / 2;
		top = y - KOOPA_BBOX_HEIGHT / 2;
		right = left + KOOPA_BBOX_WIDTH;
		bottom = top + KOOPA_BBOX_HEIGHT;
	
}

void CKoopa::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CKoopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CKoopa*>(e->obj)) return;

	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}

}

void CKoopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	if (state == KOOPA_STATE_SHELL && (GetTickCount64() - wait1 > KOOPA_WAIT_TIMEOUT * 6)) {
		SetState(KOOPA_STATE_SHELL_CHANGE);
	}
	if (state == KOOPA_STATE_SHELL_CHANGE && (GetTickCount64() - wait2 > KOOPA_WAIT_TIMEOUT)) {
		SetState(KOOPA_STATE_WALKING);
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CKoopa::Render()
{

	int aniId = ID_ANI_KOOPA_WALKING;
	if (vx >= 0) {
		aniId = ID_ANI_KOOPA_WALKING_RIGHT;
	}
	if (state == KOOPA_STATE_SHELL)
	{
		aniId = ID_ANI_KOOPA_SHELL;
	}
	if (state == KOOPA_STATE_WING_FLY) {
		aniId = ID_ANI_KOOPA_WING_FLY;
	}
	if (state == KOOPA_STATE_SHELL_MOV||state ==KOOPA_STATE_SHELL_MOV_RIGHT) {
		aniId = ID_ANI_KOOPA_SHELL;
	}
	if (state == KOOPA_STATE_SHELL_CHANGE) {
		aniId = ID_ANI_KOOPA_SHELL_CHANGE;
	}
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CKoopa::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPA_STATE_SHELL:
		y -= 1.0f;
		vx = 0;
		wait1 = GetTickCount64();
		break;
	case KOOPA_STATE_SHELL_MOV:
		vx = -KOOPA_WALKING_SPEED * 3;
		y -= 0.5f;
		break;
	case KOOPA_STATE_SHELL_MOV_RIGHT:
		vx = KOOPA_WALKING_SPEED * 3;
		y -= 0.5f;
		break;
	case KOOPA_STATE_WALKING:
		vx = -KOOPA_WALKING_SPEED;
		break;
	case KOOPA_STATE_SHELL_CHANGE:
		vx = 0;
		y -= 1;
		wait2 = GetTickCount64();
		break;
	case KOOPA_STATE_WING_FLY:

		break;
	}
}

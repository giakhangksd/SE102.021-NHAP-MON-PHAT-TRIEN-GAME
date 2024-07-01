#include "Goomba.h"
#include"Koopa.h"
#include "AssetIDs.h"


CGoomba::CGoomba(float x, float y,int type):CGameObject(x, y)
{
	this->ax = 0;
	this->ay = GOOMBA_GRAVITY;
	die_start = -1;
	wait_2_fly = -1;
	wait_2_walk = -1;
	hit_nx = 0;
	nx = -1;
	isFellDown = FALSE;
	isOnPlatform = FALSE;
	if (type == 0) {
		SetState(GOOMBA_STATE_WALKING);
	}
	if (type == 1) {
		SetState(GOOMBA_STATE_WING_WALK);
	}
}

void CGoomba::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (state == GOOMBA_STATE_DIE)
	{
		left = x - GOOMBA_BBOX_WIDTH/2;
		top = y - GOOMBA_BBOX_HEIGHT_DIE/2;
		right = left + GOOMBA_BBOX_WIDTH;
		bottom = top + GOOMBA_BBOX_HEIGHT_DIE;
	}
	else
	{ 
		left = x - GOOMBA_BBOX_WIDTH/2;
		top = y - GOOMBA_BBOX_HEIGHT/2;
		right = left + GOOMBA_BBOX_WIDTH;
		bottom = top + GOOMBA_BBOX_HEIGHT;
	}
}

void CGoomba::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CGoomba::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return; 
	if (dynamic_cast<CGoomba*>(e->obj) && e->nx!=0) return; 

		if (e->ny != 0)
		{
			vy = 0;
			if (e->ny < 0) isOnPlatform = true;
		}
		else if (e->nx != 0)
		{
			vx = -vx;
			nx = -nx;
		}

}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	if ( (state==GOOMBA_STATE_DIE) && (GetTickCount64() - die_start > GOOMBA_DIE_TIMEOUT) )
	{
		isDeleted = true;
		return;
	}
	if ((state == GOOMBA_STATE_DIE_SHELL) && (GetTickCount64() - die_start > GOOMBA_DIE_TIMEOUT))
	{
		isDeleted = true;
		return;
	}
	isOnPlatform = false;
	if ((state == GOOMBA_STATE_WING_WALK) && (GetTickCount64() - wait_2_fly > GOOMBA_DIE_TIMEOUT * 2))
	{
		SetState(GOOMBA_STATE_WING_FLY);
	}
	if ((state == GOOMBA_STATE_WING_WALK_RIGHT) && (GetTickCount64() - wait_2_fly > GOOMBA_DIE_TIMEOUT * 2))
	{
		SetState(GOOMBA_STATE_WING_FLY);
	}
	if ((state == GOOMBA_STATE_WING_FLY) && (GetTickCount64() - wait_2_walk > GOOMBA_DIE_TIMEOUT * 2) && vx < 0)
	{
		SetState(GOOMBA_STATE_WING_WALK);
	}
	if ((state == GOOMBA_STATE_WING_FLY) && (GetTickCount64() - wait_2_walk > GOOMBA_DIE_TIMEOUT * 2) && vx > 0)
	{
		SetState(GOOMBA_STATE_WING_WALK_RIGHT);
	}
	
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CGoomba::Render()
{
	
	int aniId = ID_ANI_GOOMBA_WALKING;
	if (state == GOOMBA_STATE_DIE|| state == GOOMBA_STATE_DIE_SHELL)
	{
		aniId = ID_ANI_GOOMBA_DIE;
	}
	if (state == GOOMBA_STATE_WING_WALK || state == GOOMBA_STATE_WING_WALK_RIGHT ) {
		aniId = ID_ANI_GOOMBA_WING_WALK;
	}
	if (state == GOOMBA_STATE_WING_FLY) {
		aniId = ID_ANI_GOOMBA_WING_FLY;
	}


	CAnimations::GetInstance()->Get(aniId)->Render(x,y);
	//RenderBoundingBox();
}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
		case GOOMBA_STATE_DIE:
			die_start = GetTickCount64();
			y += (GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE)/2;
			vx = 0;
			vy = 0;
			ay = 0;
			break;
		case GOOMBA_STATE_DIE_SHELL:
			die_start = GetTickCount64();
			vy = -0.2f;
			break;
		case GOOMBA_STATE_WALKING: 
			vx = -GOOMBA_WALKING_SPEED;
			y -= 1;
			break;
		case GOOMBA_STATE_WING_WALK:
			vx = -GOOMBA_WALKING_SPEED ;
			vy = 0;
			wait_2_fly = GetTickCount64();
			break;
		case GOOMBA_STATE_WING_WALK_RIGHT:
			vx = GOOMBA_WALKING_SPEED;
			vy = 0;
			wait_2_fly = GetTickCount64();
			break;
		case GOOMBA_STATE_WING_FLY:
			vy -= 0.25f;
			wait_2_walk = GetTickCount64();
			break;
	}
}

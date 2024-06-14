#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Coin.h"
#include "quesbox.h"
#include "Mushroom.h"
#include "leaf.h"
#include "Coineffect.h"
#include "Koopa.h"
#include "piranhaplant.h"

#include "Collision.h"

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	if (abs(vx) > abs(maxVx)) vx = maxVx;

	// reset untouchable timer if untouchable time has passed
	if ( GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME) 
	{
		untouchable_start = 0;
		untouchable = 0;
	}
	if (a == 1 && GetTickCount64() - spin_start > MARIO_UNTOUCHABLE_TIME / 5)
	{
		SetState(MARIO_STATE_FOX_HIT_RELEASE);
	}

	isOnPlatform = false;

	CCollision::GetInstance()->Process(this, dt, coObjects);
	if (isHolding) {
		CKoopa* koopa = dynamic_cast<CKoopa*>(koopaShell);
		koopa->UpdatePositionFollowMario();
	}
}

void CMario::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CMario::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0 && e->obj->IsBlocking() )
	{
		vy = 0;
		if (e->ny < 0) isOnPlatform = true;
	}
	else if (e->nx != 0 && e->obj->IsBlocking())
	{
		vx = 0;
	}

	if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	else if (dynamic_cast<CCoin*>(e->obj))
		OnCollisionWithCoin(e);
	else if (dynamic_cast<CQuesbox*>(e->obj))
		OnCollisionWithQuesbox(e);
	else if (dynamic_cast<CMushroom*>(e->obj))
		OnCollosionWithMushroom(e);
	else if (dynamic_cast<CLeaf*>(e->obj))
		OnCollosionWithLeaf(e);
	else if (dynamic_cast<CCoinjump*>(e->obj))
		OnCollosionWithCoineffect(e);
	else if (dynamic_cast<CKoopa*>(e->obj))
		OnCollisionWithKoopa(e);
	else if (dynamic_cast<CPlant*>(e->obj))
		OnCollisionWithPlant(e);
}

void CMario::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
	// jump on top >> kill Goomba and deflect a bit 
	if (e->ny < 0)
	{
		if (goomba->GetState() == GOOMBA_STATE_WALKING)
		{
			goomba->SetState(GOOMBA_STATE_DIE);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
		else if (goomba->GetState() == GOOMBA_STATE_WING_WALK) {
			goomba->SetState(GOOMBA_STATE_WALKING);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
		else if (goomba->GetState() == GOOMBA_STATE_WING_FLY) {
			goomba->SetState(GOOMBA_STATE_WALKING);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
	}
	else if ( isHitting == 1)
	{
		if (goomba->GetState() == GOOMBA_STATE_WALKING)
		{
			goomba->SetState(GOOMBA_STATE_DIE);
		}
		else if (goomba->GetState() == GOOMBA_STATE_WING_WALK) {
			goomba->SetState(GOOMBA_STATE_WALKING);
		}
		else if (goomba->GetState() == GOOMBA_STATE_WING_FLY) {
			goomba->SetState(GOOMBA_STATE_WALKING);
		}
	}
	else // hit by Goomba
	{
		if (untouchable == 0)
		{
			if (goomba->GetState() != GOOMBA_STATE_DIE)
			{
				if (level == MARIO_LEVEL_FOX)
				{
					level = MARIO_LEVEL_BIG;
					StartUntouchable();
				}
				else if (level == MARIO_LEVEL_BIG)
				{
					level = MARIO_LEVEL_SMALL;
					StartUntouchable();
				}
				else
				{
					SetState(MARIO_STATE_DIE);
				}
			}
		}
	}
}

void CMario::OnCollisionWithCoin(LPCOLLISIONEVENT e)
{
	e->obj->Delete();
	coin++;
}

void CMario::OnCollisionWithQuesbox(LPCOLLISIONEVENT e) {
	CQuesbox* quesbox = dynamic_cast<CQuesbox*>(e->obj);

	if (e->ny > 0) {
		if (quesbox->GetState() == QUESBOX_STATE) {
			quesbox->SetState(QUESBOX_STATE_NOT);
		}

	}
	if (e->nx < 0 || e->nx>0) {
		if (isHitting) {
			if (quesbox->GetState() == QUESBOX_STATE) {
				quesbox->SetState(QUESBOX_STATE_NOT);
			}
		}
	}
}

void CMario::OnCollosionWithMushroom(LPCOLLISIONEVENT e)
{
	CMushroom* mushroom = dynamic_cast<CMushroom*>(e->obj);
	if (mushroom->GetState() == MUSHROOM_STATE_WAITING) {
		if (e->ny > 0 && level == MARIO_LEVEL_SMALL) {
			mushroom->SetState(MUSHROOM_STATE_WALKING);
			vy = -MARIO_JUMP_DEFLECT_SPEED /2;
		}
		else if (e->ny < 0) {

		}
		//else if (e->nx < 0 || e->nx > 0) {
		//	mushroom->SetState(MUSHROOM_STATE_WALKING);
		//}
		else {
			e->obj->Delete();
		}
	}
	else {
		if (level == MARIO_LEVEL_SMALL) {
			vy = -MARIO_JUMP_DEFLECT_SPEED /2;
			level = MARIO_LEVEL_BIG;
			
		}
		e->obj->Delete();
	}
	
}

void CMario::OnCollosionWithLeaf(LPCOLLISIONEVENT e)
{
	CLeaf* leaf = dynamic_cast<CLeaf*>(e->obj);
	if(leaf->GetState()==LEAF_STATE_WAITING)
		if (e->ny > 0 && level >= MARIO_LEVEL_BIG) {
			leaf->SetState(LEAF_STATE_FALLING);
			vy = -MARIO_JUMP_DEFLECT_SPEED / 2;
		}
		else if (e->ny < 0) {

		}
		else if ((e->nx < 0 || e->nx > 0)) {
			if (isHitting) {
				leaf->SetState(LEAF_STATE_FALLING);
			}
		}
		else {
			e->obj->Delete();
		}
	else {
		if (level >= MARIO_LEVEL_BIG) {
			vy = -MARIO_JUMP_DEFLECT_SPEED / 2;
			level = MARIO_LEVEL_FOX;
		}
		e->obj->Delete();
	}

}

void CMario::OnCollosionWithCoineffect(LPCOLLISIONEVENT e)
{
	CCoinjump* coinj = dynamic_cast<CCoinjump*>(e->obj);
	if (coinj->GetState() == COIN_STATE_WAITING)
		if (e->ny > 0 ) {
			
			coinj->SetState(COIN_STATE_ACTIVE);
			coin++;
			vy = -MARIO_JUMP_DEFLECT_SPEED / 2;
		}
}

void CMario::OnCollisionWithKoopa(LPCOLLISIONEVENT e)
{
	CKoopa* koopa = dynamic_cast<CKoopa*>(e->obj);
	
	if (koopa->GetState() == KOOPA_STATE_SHELL_HOLDED) return;

	if (e->ny < 0)
	{
		if (koopa->GetState() == KOOPA_STATE_RED_WALKING)
		{
			koopa->SetState(KOOPA_STATE_SHELL);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
		else if (koopa->GetState() == KOOPA_STATE_RED_WALKING)
		{
			koopa->SetState(KOOPA_STATE_SHELL);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
		else if (koopa->GetState() == KOOPA_STATE_SHELL)
		{
			koopa->SetState(KOOPA_STATE_SHELL_MOV);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
		else if (koopa->GetState() == KOOPA_STATE_SHELL_MOV || koopa->GetState() == KOOPA_STATE_SHELL_MOV_RIGHT) {
			koopa->SetState(KOOPA_STATE_SHELL);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
		else if (koopa->GetState() == KOOPA_STATE_WING_WALK || koopa->GetState() == KOOPA_STATE_WING_FLY || koopa->GetState() == KOOPA_STATE_WING_WALK_RIGHT){
			koopa->SetState(KOOPA_STATE_RED_WALKING);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
	}
	else if (isHitting == 1)
	{
		if (koopa->GetState() == KOOPA_STATE_RED_WALKING)
		{
			koopa->SetState(KOOPA_STATE_SHELL);
		}
		else if (koopa->GetState() == KOOPA_STATE_SHELL_MOV || koopa->GetState() == KOOPA_STATE_SHELL_MOV_RIGHT) {
			koopa->SetState(KOOPA_STATE_SHELL);
		}
		else if (koopa->GetState() == KOOPA_STATE_WING_WALK || koopa->GetState() == KOOPA_STATE_WING_FLY || koopa->GetState() == KOOPA_STATE_WING_WALK_RIGHT) {
			koopa->SetState(KOOPA_STATE_RED_WALKING);
		}
	}
	else if (koopa->GetState() == KOOPA_STATE_SHELL&& !readyToHold)
	{
		if (e->nx > 0)
		{
			koopa->SetState(KOOPA_STATE_SHELL_MOV);
		}
		else if (e->nx < 0) {
			koopa->SetState(KOOPA_STATE_SHELL_MOV_RIGHT);
		}
		else {
			koopa->SetState(KOOPA_STATE_SHELL);
		}
	}
	else 
	{
		if (untouchable == 0)
		{
			if (koopa->GetState() != KOOPA_STATE_SHELL)
			{
				if (level == MARIO_LEVEL_FOX)
				{
					level = MARIO_LEVEL_BIG;
					StartUntouchable();
				}
				else if (level == MARIO_LEVEL_BIG)
				{
					level = MARIO_LEVEL_SMALL;
					StartUntouchable();
				}
				else
				{
					SetState(MARIO_STATE_DIE);
				}
			}
			else {
				if (readyToHold)
				{
					koopa->HoldByMario(&x, &y, &nx);
					koopaShell = koopa;
					SetState(MARIO_STATE_HOLDING);
				}
			}
		}
	}
}

void CMario::OnCollisionWithPlant(LPCOLLISIONEVENT e) {
	CPlant* plant = dynamic_cast<CPlant*>(e->obj);
	if (level == MARIO_LEVEL_FOX) {
		if (isHitting == 1) {
			plant->Delete();
		}

	}
	if (untouchable == 0)
	{
		if (level == MARIO_LEVEL_FOX)
		{
			level = MARIO_LEVEL_BIG;
			StartUntouchable();
		}
		else if (level == MARIO_LEVEL_BIG)
		{
			level = MARIO_LEVEL_SMALL;
			StartUntouchable();
		}
		else
		{
			SetState(MARIO_STATE_DIE);
		}

	}
}

//
// Get animation ID for small Mario
//
int CMario::GetAniIdSmall()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_SMALL_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_SMALL_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_SMALL_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_SMALL_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_SMALL_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_SMALL_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_SMALL_WALKING_LEFT;
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;

	return aniId;
}


//
// Get animdation ID for big Mario
//
int CMario::GetAniIdBig()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_WALKING_LEFT;
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_IDLE_RIGHT;

	return aniId;
}

// 
// get animation id 4 FOX MARIO
// 
int CMario::GetAniIdFox()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0) {
				
				aniId = ID_ANI_MARIO_FOX_JUMP_RUN_RIGHT;
			}
			else {
				aniId = ID_ANI_MARIO_FOX_JUMP_RUN_LEFT;
			}
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_FOX_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_FOX_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_FOX_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_FOX_SIT_LEFT;
		}
		else if (isHitting) {
			if (nx > 0) 
				aniId = ID_ANI_MARIO_FOX_HIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_FOX_HIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_FOX_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_FOX_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_FOX_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_FOX_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_FOX_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_FOX_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_FOX_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_FOX_WALKING_LEFT;
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_FOX_IDLE_RIGHT;

	return aniId;
}

void CMario::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = -1;

	if (state == MARIO_STATE_DIE)
		aniId = ID_ANI_MARIO_DIE;
	else if (level == MARIO_LEVEL_BIG)
		aniId = GetAniIdBig();
	else if (level == MARIO_LEVEL_SMALL)
		aniId = GetAniIdSmall();
	else if (level == MARIO_LEVEL_FOX)
		aniId = GetAniIdFox();

	animations->Get(aniId)->Render(x, y);

	RenderBoundingBox();
	
	DebugOutTitle(L"Coins: %d", coin);
	//DebugOutTitle(L"Score: %d", score);
}

void CMario::SetState(int state)
{
	// DIE is the end state, cannot be changed! 
	if (this->state == MARIO_STATE_DIE) return; 

	switch (state)
	{
	case MARIO_STATE_RUNNING_RIGHT:
		if (isSitting) break;
		if (isHitting) break;
		maxVx = MARIO_RUNNING_SPEED;
		ax = MARIO_ACCEL_RUN_X;
		nx = 1;
		break;
	case MARIO_STATE_RUNNING_LEFT:
		if (isSitting) break;
		if (isHitting) break;
		maxVx = -MARIO_RUNNING_SPEED;
		ax = -MARIO_ACCEL_RUN_X;
		nx = -1;
		break;
	case MARIO_STATE_WALKING_RIGHT:
		if (isSitting) break;
		if (isHitting) break;
		maxVx = MARIO_WALKING_SPEED;
		ax = MARIO_ACCEL_WALK_X;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		if (isSitting) break;
		if (isHitting) break;
		maxVx = -MARIO_WALKING_SPEED;
		ax = -MARIO_ACCEL_WALK_X;
		nx = -1;
		break;
	case MARIO_STATE_JUMP:
		if (isSitting) break;
		if (isHitting) break;
		if (level == MARIO_LEVEL_FOX) {
			if (isOnPlatform)
			{
				if (abs(this->vx) == MARIO_RUNNING_SPEED)
					vy = -MARIO_JUMP_RUN_SPEED_Y * 1.1f;
				else
					vy = -MARIO_JUMP_SPEED_Y * 1.1f;
			}
			else if (abs(this->vx) == MARIO_RUNNING_SPEED) {
				vy = -MARIO_JUMP_RUN_SPEED_Y * 0.7f;
			}
			else
				vy += -MARIO_JUMP_SPEED_Y * 0.7f;

		}
		else {
			if (isOnPlatform)
			{
				if (abs(this->vx) == MARIO_RUNNING_SPEED)
					vy = -MARIO_JUMP_RUN_SPEED_Y * 1.1f;
				else
					vy = -MARIO_JUMP_SPEED_Y * 1.1f;
			}
		}
		break;

	case MARIO_STATE_RELEASE_JUMP:
		if (vy < 0) vy += MARIO_JUMP_SPEED_Y / 2;
		break;

	case MARIO_STATE_SIT:
		if (isOnPlatform && level != MARIO_LEVEL_SMALL)
		{
			state = MARIO_STATE_IDLE;
			isSitting = true;
			vx = 0; vy = 0.0f;
			y += MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_SIT_RELEASE:
		if (isSitting)
		{
			isSitting = false;
			state = MARIO_STATE_IDLE;
			y -= MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_IDLE:
		ax = 0.0f;
		vx = 0.0f;
		break;

	case MARIO_STATE_DIE:
		vy = -MARIO_JUMP_DEFLECT_SPEED;
		vx = 0;
		ax = 0;
		break;
	case MARIO_STATE_FOX_HIT:
		if (isOnPlatform && level == MARIO_LEVEL_FOX)
		{
			isHitting = true;
			spin_start = GetTickCount64();
			a = 1;
		}
		break;
	case MARIO_STATE_FOX_HIT_RELEASE:
		if (isHitting)
		{
			isHitting = false;
		}
		break;
	case MARIO_STATE_READY_TO_HOLD:
		readyToHold = TRUE;
		break;
	case MARIO_STATE_HOLDING:
		//readyToHold = FALSE;
		isHolding = TRUE;
		break;

	case MARIO_STATE_NOT_HOLDING:
		if (isHolding)
		{
			isHolding = FALSE;
			CKoopa* koopa = dynamic_cast<CKoopa*>(koopaShell);
			SetState(MARIO_STATE_IDLE);
			if (nx < 0) {
				koopa->SetState(KOOPA_STATE_SHELL_MOV);
			}
			else {
				koopa->SetState(KOOPA_STATE_SHELL_MOV_RIGHT);
			}
			koopaShell = nullptr;
		}
		break;
	}
	CGameObject::SetState(state);
}

void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (level==MARIO_LEVEL_BIG)
	{
		if (isSitting)
		{
			left = x - MARIO_BIG_SITTING_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_SITTING_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_SITTING_BBOX_WIDTH;
			bottom = top + MARIO_BIG_SITTING_BBOX_HEIGHT;
		}
		else if (isHolding) {
			left = x - MARIO_BIG_BBOX_WIDTH;
			top = y - MARIO_BIG_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_BBOX_WIDTH * 2;
			bottom = top + MARIO_BIG_BBOX_HEIGHT;
		}
		else 
		{
			left = x - MARIO_BIG_BBOX_WIDTH/2;
			top = y - MARIO_BIG_BBOX_HEIGHT/2;
			right = left + MARIO_BIG_BBOX_WIDTH;
			bottom = top + MARIO_BIG_BBOX_HEIGHT;
		}
	}
	else if (level == MARIO_LEVEL_FOX) {
		if (isSitting)
		{
			left = x - MARIO_FOX_SITTING_BBOX_WIDTH / 2;
			top = y - MARIO_FOX_SITTING_BBOX_HEIGHT / 2;
			right = left + MARIO_FOX_SITTING_BBOX_WIDTH;
			bottom = top + MARIO_FOX_SITTING_BBOX_HEIGHT;
		}
		else if (isHitting) {
			left = x - MARIO_FOX_BBOX_WIDTH ;
			top = y - MARIO_FOX_BBOX_HEIGHT / 2;
			right = left + MARIO_FOX_BBOX_WIDTH*2;
			bottom = top + MARIO_FOX_BBOX_HEIGHT;
		}
		else if (isHolding) {
			left = x - MARIO_FOX_BBOX_WIDTH;
			top = y - MARIO_FOX_BBOX_HEIGHT / 2;
			right = left + MARIO_FOX_BBOX_WIDTH * 2;
			bottom = top + MARIO_FOX_BBOX_HEIGHT;
		}
		else
		{
			left = x - MARIO_FOX_BBOX_WIDTH / 2;
			top = y - MARIO_FOX_BBOX_HEIGHT / 2;
			right = left + MARIO_FOX_BBOX_WIDTH;
			bottom = top + MARIO_FOX_BBOX_HEIGHT;
		}
	}
	else 
	{
		left = x - MARIO_SMALL_BBOX_WIDTH/2;
		top = y - MARIO_SMALL_BBOX_HEIGHT/2;
		right = left + MARIO_SMALL_BBOX_WIDTH;
		bottom = top + MARIO_SMALL_BBOX_HEIGHT;
		if (isHolding) {
			left = x - MARIO_SMALL_BBOX_WIDTH;
			top = y - MARIO_SMALL_BBOX_HEIGHT / 2;
			right = left + MARIO_SMALL_BBOX_WIDTH * 2;
			bottom = top + MARIO_SMALL_BBOX_HEIGHT;
		}
	}

}

void CMario::SetLevel(int l)
{
	// Adjust position to avoid falling off platform
	if (this->level == MARIO_LEVEL_SMALL)
	{
		y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2;
	}
	if (this->level == MARIO_LEVEL_FOX)
	{
		y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2;
	}
	level = l;
}


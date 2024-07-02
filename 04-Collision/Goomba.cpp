#include "Goomba.h"
#include "Brick.h"
#include "Platform.h"
#include "KoopaTroopa.h"

CGoomba::CGoomba(float x, float y, int type, int l) :CGameObject(x, y) {
	ay = GOOMBA_GRAVITY;
	this->type = type;
	jump_count = 0;
	ax = 0.0;
	vx = -GOOMBA_WALKING_SPEED;
	time_start = redpara_start = -1;
	SetLevel(l);


	if (level != GOOMBA_LEVEL_PARA) SetState(GOOMBA_STATE_WALKING);
	else SetState(GOOMBA_STATE_JUMPING);
}

void CGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	if (state == GOOMBA_STATE_DIE_BY_JUMP) {
		left = x - GOOMBA_BBOX_WIDTH / 2;
		top = y - GOOMBA_BBOX_HEIGHT_DIE / 2;
		right = left + GOOMBA_BBOX_WIDTH;
		bottom = top + GOOMBA_BBOX_HEIGHT_DIE;
	}
	else {
		if (level == GOOMBA_LEVEL_PARA) {
			left = x - GOOMBA_BBOX_WIDTH / 2;
			top = y - GOOMBA_PARA_BBOX_HEIGHT / 2;
			right = left + GOOMBA_BBOX_WIDTH;
			bottom = top + GOOMBA_PARA_BBOX_HEIGHT;
		}
		else {
			left = x - GOOMBA_BBOX_WIDTH / 2;
			top = y - GOOMBA_BBOX_HEIGHT / 2;
			right = left + GOOMBA_BBOX_WIDTH;
			bottom = top + GOOMBA_BBOX_HEIGHT;
		}
	}
}

void CGoomba::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CGoomba::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0) {
		if (dynamic_cast<CPlatform*>(e->obj)) {
			CPlatform* platform = dynamic_cast<CPlatform*>(e->obj);
			switch (platform->GetType()) {
			case PLATFORM_TYPE_BLOCK:
				vy = 0;
				break;

			case PLATFORM_TYPE_NORMAL:
				if (e->ny < 0)
					vy = 0;
			}

		}
		else if (e->obj->IsBlocking()) vy = 0;
	}
	else if (e->nx != 0) {
		if (dynamic_cast<CPlatform*>(e->obj)) {
			if (dynamic_cast<CPlatform*>(e->obj)->GetType() == PLATFORM_TYPE_BLOCK)
				vx = -vx;
		}
		else if (e->obj->IsBlocking()) vx = -vx;
	}

	if (dynamic_cast<CBrick*>(e->obj))
		OnCollisionWithBrick(e);
	else if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	else if (dynamic_cast<CKoopaTroopa*>(e->obj))
		OnCollisionWithKoopaTroopa(e);
}

int CGoomba::getAniId()
{
	int aniId = -1;
	switch (type) {
	case GOOMBA_TYPE_NORMAL:
		aniId = ID_ANI_GOOMBA_WALKING;
		if (state == GOOMBA_STATE_DIE_BY_JUMP)
			aniId = ID_ANI_GOOMBA_DIE_BY_JUMP;
		else if (state == GOOMBA_STATE_DIE_BY_ATTACK)
			aniId = ID_ANI_GOOMBA_DIE_BY_ATTACK;

		break;

	case GOOMBA_TYPE_RED:
		aniId = ID_ANI_RED_GOOMBA_WALKING;
		if (level == GOOMBA_LEVEL_NORMAL) {
			if (state == GOOMBA_STATE_DIE_BY_JUMP)
				aniId = ID_ANI_RED_GOOMBA_DIE_BY_JUMP;
			else if (state == GOOMBA_STATE_DIE_BY_ATTACK)
				aniId = ID_ANI_RED_GOOMBA_DIE_BY_ATTACK;
		} else {
			if (state == GOOMBA_STATE_DIE_BY_ATTACK)
				aniId = ID_ANI_RED_GOOMBA_DIE_BY_ATTACK;
			else if (state == GOOMBA_STATE_JUMPING)
				aniId = ID_ANI_RED_PARA_GOOMBA_JUMPING;
			else if (state == GOOMBA_STATE_FLYING)
				aniId = ID_ANI_RED_PARA_GOOMBA_FLYING;
			else if (state == GOOMBA_STATE_WALKING)
				aniId = ID_ANI_RED_PARA_GOOMBA_WALKING;
		}
		break;
	}

	return aniId;
}

void CGoomba::OnCollisionWithBrick(LPCOLLISIONEVENT e)
{
	CBrick* brick = dynamic_cast<CBrick*>(e->obj);

	if (brick->IsAttacking() && brick->IsBrokenByJump() && e->ny != 0) {
		SetState(GOOMBA_STATE_DIE_BY_ATTACK);

		float bx, by;
		brick->GetPosition(bx, by);

		if (bx < x)
			this->Deflected(DEFLECT_DIRECTION_RIGHT);
		else this->Deflected(DEFLECT_DIRECTION_LEFT);
	}
}

void CGoomba::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

	if (goomba->state == GOOMBA_STATE_DIE_BY_JUMP || goomba->state == GOOMBA_STATE_DIE_BY_ATTACK)
		return;

	if (e->nx != 0) goomba->vx = -goomba->vx;
}

void CGoomba::OnCollisionWithKoopaTroopa(LPCOLLISIONEVENT e) {
	CKoopaTroopa* koopa = dynamic_cast<CKoopaTroopa*>(e->obj);

	switch (koopa->GetState()) {
	case KOOPA_TROOPA_STATE_ATTACKING:

		SetState(GOOMBA_STATE_DIE_BY_ATTACK);

		if (e->nx >= 0) Deflected(DEFLECT_DIRECTION_RIGHT);
		else Deflected(DEFLECT_DIRECTION_LEFT);

		break;

	case KOOPA_TROOPA_STATE_SHELL:
		if (koopa->GetStateHeld()) {
			koopa->SetState(KOOPA_TROOPA_STATE_DIE);
			this->SetState(GOOMBA_STATE_DIE_BY_ATTACK);
		}

		break;
	}
}

void CGoomba::SetLevel(int l)
{
	if (level == GOOMBA_LEVEL_PARA)
		y -= (GOOMBA_PARA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT);
	level = l;
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	if ((state == GOOMBA_STATE_DIE_BY_JUMP || state == GOOMBA_STATE_DIE_BY_ATTACK) &&
		(GetTickCount64() - time_start > GOOMBA_DIE_TIMEOUT))
	{
		isDeleted = true;
		return;
	}

	if (level == GOOMBA_LEVEL_PARA) {
		switch (state) {
		case GOOMBA_STATE_JUMPING:
			if (jump_count >= 3) {
				SetState(GOOMBA_STATE_FLYING);
				jump_count = 0;
				redpara_start = GetTickCount64();
			}
			else if (vy == ay * dt && GetTickCount64() - redpara_start > GOOMBA_RED_PARA_RELEASE_JUMP_TIME)
				SetState(GOOMBA_STATE_JUMPING);
			break;

		case GOOMBA_STATE_FLYING:
			if (redpara_start == -1) {
				SetState(GOOMBA_STATE_WALKING);
				redpara_start = GetTickCount64();
			}
			else if (vy == ay * dt) redpara_start = -1;
			break;

		case GOOMBA_STATE_WALKING:
			if (GetTickCount64() - redpara_start >= GOOMBA_RED_PARA_WALK_TIME) {
				redpara_start = -1;
				SetState(GOOMBA_STATE_JUMPING);
			}
			break;
		}
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CGoomba::Render()
{
	int aniId = getAniId();

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CGoomba::Deflected(int direction)
{
	vy = -GOOMBA_DIE_DEFLECT;
	ay = GOOMBA_GRAVITY;

	vx = direction * GOOMBA_WALKING_SPEED;
	ax = 0;
}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GOOMBA_STATE_DIE_BY_JUMP:
		if (level == GOOMBA_LEVEL_PARA) {
			SetLevel(GOOMBA_LEVEL_NORMAL);
			SetState(GOOMBA_STATE_WALKING);
			return;
		}

		time_start = GetTickCount64();
		y += (GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE) / 2;
		vx = vy = ay = 0;
		break;

	case GOOMBA_STATE_DIE_BY_ATTACK:
		time_start = GetTickCount64();
		Deflected(0);
		break;

	case GOOMBA_STATE_WALKING:
		vx = -GOOMBA_WALKING_SPEED;
		break;

	case GOOMBA_STATE_JUMPING:
		redpara_start = GetTickCount64();
		vy = -GOOMBA_JUMP_SPEED_Y;
		jump_count += 1;
		break;

	case GOOMBA_STATE_FLYING:
		vy = -GOOMBA_FLY_SPEED_Y;
		break;
	}
}

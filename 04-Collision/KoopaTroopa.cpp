#include "Brick.h"
#include "Goomba.h"
#include "Platform.h"
#include "PlayScene.h"
#include "KoopaTroopa.h"
#include "PiranhaPlant.h"
#include "debug.h"

void CKoopaTroopa::Deflected(int direction)
{
	vy = -KOOPA_TROOPA_DIE_DEFLECT;
	ay = KOOPA_TROOPA_GRAVITY;

	vx = direction * KOOPA_TROOPA_WALKING_SPEED;
	deflected_start = GetTickCount64();

	isUp = true;
	if (level == KOOPA_TROOPA_LEVEL_PARA) level = KOOPA_TROOPA_LEVEL_NORMAL;
}

void CKoopaTroopa::OnCollisionWithBrick(LPCOLLISIONEVENT e)
{
	CBrick* brick = dynamic_cast<CBrick*>(e->obj);

	if (brick->IsAttacking() && brick->IsBrokenByJump() && e->ny != 0) {
		this->SetState(KOOPA_TROOPA_STATE_SHELL);

		float bx, by;
		brick->GetPosition(bx, by);

		if (bx < x)
			this->Deflected(DEFLECT_DIRECTION_RIGHT);
		else
			this->Deflected(DEFLECT_DIRECTION_LEFT);
	}
	if (state == KOOPA_TROOPA_STATE_ATTACKING && e->nx != 0)
	{
		if (brick->GetType() == BRICK_TYPE_GOLD)
			brick->SetType(BRICK_TYPE_BREAK);
		else if (brick->GetType() == BRICK_TYPE_QUESTION)
			brick->SetType(BRICK_TYPE_EMPTY);
	}
}

void CKoopaTroopa::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

	if (goomba->GetState() == GOOMBA_STATE_DIE_BY_JUMP || goomba->GetState() == GOOMBA_STATE_DIE_BY_ATTACK)
		return;

	switch (state) {
	case KOOPA_TROOPA_STATE_SHELL:
	case KOOPA_TROOPA_STATE_WALKING:
		float gvx, gvy;
		goomba->GetSpeed(gvx, gvy);
		goomba->SetSpeed(-gvx, gvy);

		this->vx = -this->vx;

		float p_vx, p_vy;
		phaseCheck->GetSpeed(p_vx, p_vy);

		if (p_vx >= this->vx) phaseCheck->SetPosition(x - KOOPA_TROOPA_BBOX_WIDTH, y);
		else phaseCheck->SetPosition(x + KOOPA_TROOPA_BBOX_WIDTH, y);

		break;

	case KOOPA_TROOPA_STATE_ATTACKING:
		goomba->SetState(GOOMBA_STATE_DIE_BY_ATTACK);

		float gx, gy;
		goomba->GetPosition(gx, gy);

		if (gx >= x)
			goomba->Deflected(DEFLECT_DIRECTION_RIGHT);
		else goomba->Deflected(DEFLECT_DIRECTION_LEFT);

		break;
	}
}

void CKoopaTroopa::OnCollisionWithKoopaTroopa(LPCOLLISIONEVENT e)
{
	CKoopaTroopa* koopa = dynamic_cast<CKoopaTroopa*>(e->obj);

	if (state == KOOPA_TROOPA_STATE_DIE || koopa->GetState() == KOOPA_TROOPA_STATE_DIE)
		return;

	int flag = 0;
	if (koopa->state == KOOPA_TROOPA_STATE_ATTACKING) flag = 1;
	else if (koopa->state == KOOPA_TROOPA_STATE_SHELL && koopa->isHeld) flag = 2;

	switch (state) {
	case KOOPA_TROOPA_STATE_ATTACKING:
		koopa->SetState(KOOPA_TROOPA_STATE_DIE);
		if(flag) SetState(KOOPA_TROOPA_STATE_DIE);

		if (koopa->x >= x) {
			if (state == KOOPA_TROOPA_STATE_DIE) Deflected(DEFLECT_DIRECTION_LEFT);
			koopa->Deflected(DEFLECT_DIRECTION_RIGHT);
		} else {
			if (state == KOOPA_TROOPA_STATE_DIE) Deflected(DEFLECT_DIRECTION_RIGHT);
			koopa->Deflected(DEFLECT_DIRECTION_LEFT);
		}

		break;

	case KOOPA_TROOPA_STATE_SHELL:
	case KOOPA_TROOPA_STATE_FLYING:
	case KOOPA_TROOPA_STATE_WALKING:
		vx = -vx;

		if (vx < 0) phaseCheck->SetPosition(x - KOOPA_TROOPA_BBOX_WIDTH, y);
		else phaseCheck->SetPosition(x + KOOPA_TROOPA_BBOX_WIDTH, y);

		if (koopa->state == KOOPA_TROOPA_STATE_WALKING) {
			koopa->vx = -koopa->vx;
			if (koopa->vx < 0) koopa->phaseCheck->SetPosition(koopa->x - KOOPA_TROOPA_BBOX_WIDTH, koopa->y);
			else koopa->phaseCheck->SetPosition(koopa->x + KOOPA_TROOPA_BBOX_WIDTH, koopa->y);
		}

		if (flag) {
			SetState(KOOPA_TROOPA_STATE_DIE);
			if (flag == 2) koopa->SetState(KOOPA_TROOPA_STATE_DIE);
			else {
				if (x >= koopa->x) Deflected(DEFLECT_DIRECTION_RIGHT);
				else Deflected(DEFLECT_DIRECTION_LEFT);
			}
		}

		break;
	}
}

void CKoopaTroopa::OnCollisionWithPiranhaPlant(LPCOLLISIONEVENT e) {
	if (state == KOOPA_TROOPA_STATE_ATTACKING)
		e->obj->Delete();
}

int CKoopaTroopa::GetAniId()
{
	int aniId = ID_ANI_RED_KOOPA_TROOPA_WALKING_LEFT;

	switch (type)
	{
	case KOOPA_TROOPA_TYPE_RED:
		if (vx >= 0) aniId = ID_ANI_RED_KOOPA_TROOPA_WALKING_RIGHT;
		if (state == KOOPA_TROOPA_STATE_DIE) aniId = ID_ANI_RED_KOOPA_TROOPA_SHELL_UP;

		else if (state == KOOPA_TROOPA_STATE_SHELL) {
			switch (isUp) {
			case true:
				aniId = ID_ANI_RED_KOOPA_TROOPA_SHELL_UP;
				if (GetTickCount64() - time_start >= 4000)
					aniId = ID_ANI_RED_KOOPA_TROOPA_REFORM_UP;
				break;

			case false:
				aniId = ID_ANI_RED_KOOPA_TROOPA_SHELL_DOWN;
				if (GetTickCount64() - time_start >= 4000)
					aniId = ID_ANI_RED_KOOPA_TROOPA_REFORM_DOWN;
				break;
			}
		}
		else if (state == KOOPA_TROOPA_STATE_ATTACKING) 
			if (isUp) aniId = ID_ANI_RED_KOOPA_TROOPA_ATTACKING_UP;
			else aniId = ID_ANI_RED_KOOPA_TROOPA_ATTACKING_DOWN;
		break;

	case KOOPA_TROOPA_TYPE_GREEN:
		if (level == KOOPA_TROOPA_LEVEL_NORMAL) {
			aniId = ID_ANI_GREEN_KOOPA_TROOPA_WALKING_LEFT;
			if (vx >= 0) aniId = ID_ANI_GREEN_KOOPA_TROOPA_WALKING_RIGHT;

			if (state == KOOPA_TROOPA_STATE_DIE) aniId = ID_ANI_GREEN_KOOPA_TROOPA_SHELL_UP;
			else if (state == KOOPA_TROOPA_STATE_SHELL) {
				switch (isUp) {
				case true:
					aniId = ID_ANI_GREEN_KOOPA_TROOPA_SHELL_UP;
					if (GetTickCount64() - time_start >= 4000)
						aniId = ID_ANI_GREEN_KOOPA_TROOPA_REFORM_UP;
					break;

				case false:
					aniId = ID_ANI_GREEN_KOOPA_TROOPA_SHELL_DOWN;
					if (GetTickCount64() - time_start >= 4000)
						aniId = ID_ANI_GREEN_KOOPA_TROOPA_REFORM_DOWN;
					break;
				}
			}
			else if (state == KOOPA_TROOPA_STATE_ATTACKING)
				if (isUp) aniId = ID_ANI_GREEN_KOOPA_TROOPA_ATTACKING_UP;
				else aniId = ID_ANI_GREEN_KOOPA_TROOPA_ATTACKING_DOWN;
		}
		else {
			if (vx >= 0) aniId = ID_ANI_GREEN_PARA_KOOPA_TROOPA_FLYING_RIGHT;
			else aniId = ID_ANI_GREEN_PARA_KOOPA_TROOPA_FLYING_LEFT;
		}
	}

	return aniId;
}

CKoopaTroopa::CKoopaTroopa(float x, float y, int type, int l) :CGameObject(x, y) {
	phaseCheck = new CPhaseChecker(x - KOOPA_TROOPA_BBOX_WIDTH - KOOPA_TROOPA_PHASE_CHECK_WIDTH / 2, y,
		KOOPA_TROOPA_PHASE_CHECK_WIDTH, KOOPA_TROOPA_PHASE_CHECK_HEIGHT);
	phaseCheck->SetSpeed(0, KOOPA_TROOPA_WALKING_SPEED);

	this->type = type;
	this->level = l;
	ay = KOOPA_TROOPA_GRAVITY;
	time_start = -1;

	if (level == KOOPA_TROOPA_LEVEL_PARA)
		SetState(KOOPA_TROOPA_STATE_FLYING);
	else SetState(KOOPA_TROOPA_STATE_WALKING);

	vx = -KOOPA_TROOPA_WALKING_SPEED;
	isUp = isHeld = false;
	deflected_start = 0;
}

void CKoopaTroopa::SetState(int state)
{
	CGameObject::SetState(state);
	isHeld = false;
	if (state != KOOPA_TROOPA_STATE_ATTACKING && state != KOOPA_TROOPA_STATE_SHELL) isUp = false;
	switch (state) {
	case KOOPA_TROOPA_STATE_WALKING:
		vx = -KOOPA_TROOPA_WALKING_SPEED;
		phaseCheck->SetPosition(x - KOOPA_TROOPA_BBOX_WIDTH - KOOPA_TROOPA_PHASE_CHECK_WIDTH / 2, y);
		break;

	case KOOPA_TROOPA_STATE_SHELL:
		time_start = GetTickCount64();
		y += (KOOPA_TROOPA_BBOX_HEIGHT - KOOPA_TROOPA_BBOX_HEIGHT_DIE) / 2;
		vx = vy = 0;
		break;

	case KOOPA_TROOPA_STATE_ATTACKING:
		time_start = -1;
		if (nx >= 0) vx = -KOOPA_TROOPA_SHELL_SPEED;
		else vx = KOOPA_TROOPA_SHELL_SPEED;
		break;

	case KOOPA_TROOPA_STATE_FLYING:
		time_start = GetTickCount64();
		vy = -KOOPA_TROOPA_FLY_SPEED_Y;
		break;

	case KOOPA_TROOPA_STATE_DIE:
		time_start = GetTickCount64();
		phaseCheck->Delete();
		break;
	}

}

void CKoopaTroopa::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CKoopaTroopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (state == KOOPA_TROOPA_STATE_SHELL && deflected_start == 0) vx = 0;

	if (e->ny != 0) {
		if (dynamic_cast<CPlatform*>(e->obj)) {
			CPlatform* platform = dynamic_cast<CPlatform*>(e->obj);

			switch (platform->GetType()) {
			case PLATFORM_TYPE_BLOCK:
				vy = 0;
				break;

			case PLATFORM_TYPE_NORMAL:
				if (e->ny < 0) vy = 0;
				break;
			}
		}
		else if (e->obj->IsBlocking()) vy = 0;
	}
	else if (e->nx != 0) {
		if (dynamic_cast<CPlatform*>(e->obj)) {
			if (dynamic_cast<CPlatform*>(e->obj)->GetType() == PLATFORM_TYPE_BLOCK) {
				vx = -vx;

				float p_vx, p_vy;
				phaseCheck->GetSpeed(p_vx, p_vy);

				if (p_vx >= this->vx)
					phaseCheck->SetPosition(x - KOOPA_TROOPA_BBOX_WIDTH, y);
				else
					phaseCheck->SetPosition(x + KOOPA_TROOPA_BBOX_WIDTH, y);
			}
		}
		else if (e->obj->IsBlocking()) {
			vx = -vx;

			float p_vx, p_vy;
			phaseCheck->GetSpeed(p_vx, p_vy);

			if (p_vx >= this->vx)
				phaseCheck->SetPosition(x - KOOPA_TROOPA_BBOX_WIDTH, y);
			else
				phaseCheck->SetPosition(x + KOOPA_TROOPA_BBOX_WIDTH, y);
		}
	}

	float px, py;
	phaseCheck->GetPosition(px, py);

	if (py - this->y > 10 && level != KOOPA_TROOPA_LEVEL_PARA && state != KOOPA_TROOPA_STATE_SHELL && state != KOOPA_TROOPA_STATE_ATTACKING) {
		vx = -vx;

		if (px <= this->x)
			phaseCheck->SetPosition(x + KOOPA_TROOPA_BBOX_WIDTH, y);
		else phaseCheck->SetPosition(x - KOOPA_TROOPA_BBOX_WIDTH, y);
	}

	phaseCheck->SetSpeed(vx, 1);

	if (dynamic_cast<CBrick*>(e->obj))
		OnCollisionWithBrick(e);
	if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	else if (dynamic_cast<CKoopaTroopa*>(e->obj))
		OnCollisionWithKoopaTroopa(e);
	else if (dynamic_cast<CPiranhaPlant*>(e->obj))
		OnCollisionWithPiranhaPlant(e);
}

void CKoopaTroopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == KOOPA_TROOPA_STATE_WALKING) {
		left = x - KOOPA_TROOPA_BBOX_WIDTH / 2;
		top = y - KOOPA_TROOPA_BBOX_HEIGHT / 2;
		right = left + KOOPA_TROOPA_BBOX_WIDTH;
		bottom = top + KOOPA_TROOPA_BBOX_HEIGHT;
	}
	else {
		left = x - KOOPA_TROOPA_BBOX_WIDTH / 2;
		top = y - KOOPA_TROOPA_BBOX_HEIGHT_DIE / 2;
		right = left + KOOPA_TROOPA_BBOX_WIDTH;
		bottom = top + KOOPA_TROOPA_BBOX_HEIGHT_DIE;
	}
}

void CKoopaTroopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (level == KOOPA_TROOPA_LEVEL_PARA) {
		if (((LPPLAYSCENE)(CGame::GetInstance()->GetCurrentScene()))) {
			CInvisibleObject* invisibleObject = ((LPPLAYSCENE)(CGame::GetInstance()->GetCurrentScene()))->GetInvisibleObject();
			if (invisibleObject && !invisibleObject->IsActived()) return;
		}
	}

	if (deflected_start && vy == 0) deflected_start = 0;

	if (state == KOOPA_TROOPA_STATE_WALKING) phaseCheck->Update(dt, coObjects);

	if (isHeld) ay = 0;
	else ay = KOOPA_TROOPA_GRAVITY;

	vy += ay * dt;

	if ((state == KOOPA_TROOPA_STATE_SHELL) && (GetTickCount64() - time_start > KOOPA_TROOPA_SHELL_TIMEOUT)) {
		SetState(KOOPA_TROOPA_STATE_WALKING);
		y -= (KOOPA_TROOPA_BBOX_HEIGHT - KOOPA_TROOPA_BBOX_HEIGHT_DIE);
		time_start = -1;
		return;
	}
	else if ((state == KOOPA_TROOPA_STATE_DIE) && (GetTickCount64() - time_start > KOOPA_TROOPA_DIE_TIMEOUT)) {
		isDeleted = true;
		phaseCheck->Delete();
		return;
	} else if (state == KOOPA_TROOPA_STATE_FLYING && 
		(GetTickCount64() - time_start) > KOOPA_TROOPA_FLY_TIMEOUT && 
		vy == ay * dt) {
		time_start = -1;
		SetState(KOOPA_TROOPA_STATE_FLYING);
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CKoopaTroopa::Render()
{
	//phaseCheck->RenderBoundingBox();

	int aniId = GetAniId();
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();

	float px, py;
	phaseCheck->GetPosition(px, py);
}

#include "PiranhaPlant.h"
#include "Game.h"
#include "debug.h"
#include "Mario.h"
#include "PlayScene.h"
#include "KoopaTroopa.h"

void CPiranhaPlant::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (type == PIRANHA_TYPE_RED_FIRE) {
		l = x - PIRANHA_RED_BBOX_WIDTH / 2;
		t = y - PIRANHA_RED_BBOX_HEIGHT / 2;
		r = l + PIRANHA_RED_BBOX_WIDTH;
		b = t + PIRANHA_RED_BBOX_HEIGHT;
	}
	else {
		l = x - PIRANHA_GREEN_BBOX_WIDTH / 2;
		t = y - PIRANHA_GREEN_BBOX_HEIGHT / 2;
		r = l + PIRANHA_GREEN_BBOX_WIDTH;
		b = t + PIRANHA_GREEN_BBOX_HEIGHT;
	}
}
void CPiranhaPlant::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CKoopaTroopa*>(e->obj)) {
		CKoopaTroopa* koopa = dynamic_cast<CKoopaTroopa*>(e->obj);

		if (koopa->GetState() == KOOPA_TROOPA_STATE_SHELL && koopa->GetStateHeld()) {
			Delete();
			koopa->SetState(KOOPA_TROOPA_STATE_DIE);
		}
	}
	else if (dynamic_cast<CPhaseChecker*>(e->obj)) Delete();
}

void CPiranhaPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	y += vy * dt;

	switch (state) {
	case PIRANHA_STATE_UP:
		switch (type) {
		case PIRANHA_TYPE_GREEN:
		case PIRANHA_TYPE_GREEN_FIRE:
			if (old_y - y > PIRANHA_GREEN_BBOX_HEIGHT) {
				y = old_y - PIRANHA_GREEN_BBOX_HEIGHT;
				SetState(PIRANHA_STATE_ATTACK);
			}
			break;

		case PIRANHA_TYPE_RED_FIRE:
			if (old_y - y > PIRANHA_RED_BBOX_HEIGHT) {
				y = old_y - PIRANHA_RED_BBOX_HEIGHT;
				SetState(PIRANHA_STATE_ATTACK);
			}
			break;
		}
		break;

	case PIRANHA_STATE_DOWN:
		if (y - old_y >= 0) {
			y = old_y;
			SetState(PIRANHA_STATE_IDLE);
		}
		break;

	case PIRANHA_STATE_ATTACK:
	case PIRANHA_STATE_IDLE:
		if ((GetTickCount64() - time_start) > PIRANHA_ATTACK_IDLE_TIME) {
			time_start = 0;
			if (state == PIRANHA_STATE_IDLE && abs(mario_x - x) > DISTANCE_APPEAR) SetState(PIRANHA_STATE_UP);
			else if (state == PIRANHA_STATE_ATTACK) SetState(PIRANHA_STATE_DOWN);
		}
		break;
	}

	//Get position of Mario
	CPlayScene* playScene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
	playScene->GetPlayer()->GetPosition(mario_x, mario_y);


	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}
int CPiranhaPlant::GetAniId()
{
	int aniId = -1;
	switch (type) {
	case PIRANHA_TYPE_GREEN:
		aniId = ID_ANI_PIRANHA_GREEN;
		break;

	case PIRANHA_TYPE_GREEN_FIRE:
		if (mario_x < x) {
			if (mario_y < y) aniId = ID_ANI_PIRANHA_GREEN_TARGET_TOP_LEFT;
			else aniId = ID_ANI_PIRANHA_GREEN_TARGET_BOTTOM_LEFT;
		}
		else {
			if (mario_y < y) aniId = ID_ANI_PIRANHA_GREEN_TARGET_TOP_RIGHT;
			else aniId = ID_ANI_PIRANHA_GREEN_TARGET_BOTTOM_RIGHT;
		}
		break;

	case PIRANHA_TYPE_RED_FIRE:
		if (mario_x < x) {
			if (mario_y < y) aniId = ID_ANI_PIRANHA_RED_TARGET_TOP_LEFT;
			else aniId = ID_ANI_PIRANHA_RED_TARGET_BOTTOM_LEFT;
		}
		else {
			if (mario_y < y) aniId = ID_ANI_PIRANHA_RED_TARGET_TOP_RIGHT;
			else aniId = ID_ANI_PIRANHA_RED_TARGET_BOTTOM_RIGHT;
		}
		break;
	}
	return aniId;
}
void CPiranhaPlant::Render()
{
	int aniId = GetAniId();

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CPiranhaPlant::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case PIRANHA_STATE_UP:
		vy = -PIRANHA_UP_DOWN_SPEED;
		break;

	case PIRANHA_STATE_DOWN:
		vy = PIRANHA_UP_DOWN_SPEED;
		break;

	case PIRANHA_STATE_ATTACK: {
		time_start = GetTickCount64();
		vy = 0;

		if (type == PIRANHA_TYPE_GREEN)
			return;

		CGameObject* _bullet = new CBullet(x, y, BULLET_BY_PIRANHA);

		float direction_x, direction_y;
		direction_x = (mario_x >= x) ? BULLET_DIRECTION_RIGHT : BULLET_DIRECTION_LEFT;
		direction_y = (mario_y >= y) ? BULLET_DIRECTION_BOTTOM : BULLET_DIRECTION_TOP;

		dynamic_cast<CBullet*>(_bullet)->SetDirection(direction_x, direction_y);

		((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetObjects().push_back(_bullet);

		break;
	}
	case PIRANHA_STATE_IDLE:
		time_start = GetTickCount64();
		vy = 0;
		break;
	}
}
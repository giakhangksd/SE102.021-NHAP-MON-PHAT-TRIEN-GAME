#include "Hud.h"
#include "Mario.h"
#include "Effect.h"
#include "Mushroom.h"
#include "Platform.h"
#include "PlayScene.h"

void CMushroom::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	// Because coordinate is in center of an object, we calculate like bellow to have bbox
	left = x - MUSHROOM_BBOX_WIDTH / 2;
	top = y - MUSHROOM_BBOX_HEIGHT / 2;
	right = left + MUSHROOM_BBOX_WIDTH;
	bottom = top + MUSHROOM_BBOX_HEIGHT;
}

void CMushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;

	switch (type) {
	case MUSHROOM_TYPE_SUPER_LEAF: {
		if (vy > 0) {
			if (vx == 0) {
				vx = MUSHROOM_WALKING_SPEED;
				time_start = GetTickCount64();
			}

			IsDiversion();
			vy = ay * dt;

			vector<LPGAMEOBJECT> object;
			if (dynamic_cast<LPPLAYSCENE>(CGame::GetInstance()->GetCurrentScene())) {
				CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
				object.push_back(mario);

				CCollision::GetInstance()->Process(this, dt, &object);
			}
			else CCollision::GetInstance()->Process(this, dt, &object);
			return;
		}
	}
		break;

	default: {
		if ((old_y - y) >= MUSHROOM_DEFLECT_MAX_HEIGHT) {
			ay = MUSHROOM_GRAVITY;
			vx = MUSHROOM_WALKING_SPEED;
		}
	}
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);

	/*DebugOut(L"x = %f, vx = %f, y = %f, vy = %f, ay = %f, dt = %f\n", x, vx, y, vy, ay, dt);*/
}

void CMushroom::Render()
{
	int spriteId;
	if (type == MUSHROOM_TYPE_SUPER)
		spriteId = ID_SPRITE_MUSHROOM_SUPER;
	else if (type == MUSHROOM_TYPE_1UP)
		spriteId = ID_SPRITE_MUSHROOM_1UP;
	else {
		if (vx > 0) spriteId = ID_SPRITE_SUPER_LEAF_RIGHT;
		else spriteId = ID_SPRITE_SUPER_LEAF_LEFT;
	}
	CSprites::GetInstance()->Get(spriteId)->Draw(x, y);
	//RenderBoundingBox();
}

void CMushroom::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CMushroom::OnColisionWith(LPCOLLISIONEVENT e)
{
	if (isDeleted) return;

	if (dynamic_cast<CMario*>(e->obj)) {
		CMario* mario = dynamic_cast<CMario*>(e->obj);

		if (type == MUSHROOM_TYPE_1UP) {
			CHud::GetInstance()->Collect1UpMushroom();
			CEffect::GetInstance()->pushEffectIntoQueue(x, y, ID_SPRITE_POINTS_1UP, true, true);
		}
		else {
			switch (mario->GetLevel()) {
			if (mario->GetLevel() != MARIO_LEVEL_RACOON)
				mario->SetTransformStart();

			case MARIO_LEVEL_SMALL:
				mario->SetLevel(MARIO_LEVEL_BIG);
				break;

			case MARIO_LEVEL_BIG:
				mario->SetLevel(MARIO_LEVEL_RACOON);
				break;
			}
			CHud::GetInstance()->CollectScore(SCORE_SUPER_MUSHROOM_LEAF);
			CEffect::GetInstance()->pushEffectIntoQueue(x, y, ID_SPRITE_POINTS_1000, true, true);
		}
		Delete();
	}
	else if (dynamic_cast<CBrick*>(e->obj)) {
		CBrick* brick = dynamic_cast<CBrick*>(e->obj);
		if (brick->IsAttacking()) {
			Deflected();
			vx = -vx;
		}
	}

	if (!e->obj->IsBlocking() || dynamic_cast<CGoomba*>(e->obj) || dynamic_cast<CMushroom*>(e->obj))
		return;

	if (e->ny != 0)
		vy = 0;
	else if (e->nx != 0)
		vx = -vx;
}

void CMushroom::IsDiversion() {
	if (GetTickCount64() - time_start >= LEAF_DIVERT_TIME) {
		vx = -vx;
		time_start = GetTickCount64();
	}
}

void CMushroom::Deflected(int direction) {
	vy = -MUSHROOM_DEFLECTED_SPEED;
	vx *= direction;
}

CMushroom::CMushroom(float x, float y, int type) :CGameObject(x, y) {
	old_y = 0.0;
	time_start = 0;
	this->type = type;

	if (type == MUSHROOM_TYPE_SUPER_LEAF)
		time_start = GetTickCount64();

	this->ay = MUSHROOM_GRAVITY;
	vx = MUSHROOM_WALKING_SPEED;
}

void CMushroom::CreatedByBrick() {
	old_y = y;

	if (type == MUSHROOM_TYPE_SUPER_LEAF) {
		Deflected();
		return;
	}

	ay = 0;
	vx = 0;
	vy = -MUSHROOM_CREATE_SPEED;
}

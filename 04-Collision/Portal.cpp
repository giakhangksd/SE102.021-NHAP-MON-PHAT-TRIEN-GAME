#include "Game.h"
#include "Mario.h"
#include "Timer.h"
#include "Portal.h"
#include "Textures.h"
#include "PlayScene.h"

CPortal::CPortal(float l, float t, float r, float b, int scene_id, int Type)
{
	this->scene_id = scene_id;
	x = l;
	y = t;
	vy = 0;
	type = Type;
	width = r - l;
	height = b - t;
	switchScene_start = 0;
	giftType = GIFT_TYPE_MUSHROOM;
	switchGift_start = GetTickCount64();
	goal_x = x - GOAL_WIDTH / 10;
	goal_y = y - GOAL_HEIGHT / 1.3f;
}

void CPortal::RenderBoundingBox()
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPTEXTURE bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);

	float l, t, r, b;

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	float cx, cy;
	CGame::GetInstance()->GetCamPos(cx, cy);

	CGame::GetInstance()->Draw(x - cx, y - cy, bbox, nullptr, BBOX_ALPHA, rect.right - 1, rect.bottom - 1);
}

void CPortal::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	y += vy * dt;

	//switching gift
	if (GetTickCount64() - switchGift_start > SWITCHING_GIFT_TIME) {
		switchGift_start = GetTickCount64();

		if (!switchScene_start)
			switch (giftType) {
			case GIFT_TYPE_MUSHROOM:
				giftType = GIFT_TYPE_FLOWER;
				break;

			case GIFT_TYPE_FLOWER:
				giftType = GIFT_TYPE_STAR;
				break;

			case GIFT_TYPE_STAR:
				giftType = GIFT_TYPE_MUSHROOM;
				break;
			}
	}

	if (switchScene_start) SwitchScene();
}

void CPortal::Render() {
	//RenderBoundingBox();

	if (type == PORTAL_TYPE_ANOTHER_TO_PLAYSCENE) return;

	if (!switchScene_start) {
		CSprites* sprites = CSprites::GetInstance();

		switch (giftType) {
		case GIFT_TYPE_MUSHROOM:
			sprites->Get(ID_SPRITE_GIFT_MUSHROOM)->Draw(x, y);
			break;

		case GIFT_TYPE_FLOWER:
			sprites->Get(ID_SPRITE_GIFT_FLOWER)->Draw(x, y);
			break;

		case GIFT_TYPE_STAR:
			sprites->Get(ID_SPRITE_GIFT_STAR)->Draw(x, y);
			break;
		}
	}
	else {
		CSprites* sprites = CSprites::GetInstance();
		sprites->Get(ID_SPRITE_GOAL)->Draw(goal_x, goal_y);

		CAnimations* animations = CAnimations::GetInstance();

		switch (giftType) {
		case GIFT_TYPE_MUSHROOM:
			animations->Get(ID_ANI_GIFT_MUSHROOM)->Render(x, y);
			sprites->Get(ID_SPRITE_GIFT_MUSHROOM_ON_HUD)->Draw(goal_x + GOAL_WIDTH / 4 - 1, goal_y + GOAL_HEIGHT / 4 + 2);

			break;

		case GIFT_TYPE_FLOWER:
			animations->Get(ID_ANI_GIFT_FLOWER)->Render(x, y);
			sprites->Get(ID_SPRITE_GIFT_FLOWER_ON_HUD)->Draw(goal_x + GOAL_WIDTH / 4 - 1, goal_y + GOAL_HEIGHT / 4 + 2);

			break;

		case GIFT_TYPE_STAR:
			animations->Get(ID_ANI_GIFT_STAR)->Render(x, y);
			sprites->Get(ID_SPRITE_GIFT_STAR_ON_HUD)->Draw(goal_x + GOAL_WIDTH / 4 - 1, goal_y + GOAL_HEIGHT / 4 + 2);

			break;
		}
	}
}

void CPortal::GetBoundingBox(float& l, float& t, float& r, float& b) {
	l = x - width / 2;
	t = y - height / 2;
	r = x + width / 2;
	b = y + height / 2;
}

void CPortal::SwitchScene() {
	if (type == PORTAL_TYPE_ANOTHER_TO_PLAYSCENE) {
		CTimer::GetInstance()->InitTime();
		CGame::GetInstance()->InitiateSwitchScene(scene_id);
	}

	if (!switchScene_start) {
		switchScene_start = GetTickCount64();
		vy = -GIFT_FLYING_SPEED;
	}
	else if (GetTickCount64() - switchScene_start > SWITCHING_SCENE_TIME) {
		switchScene_start = 0;
		vy = 0;
		CTimer::GetInstance()->InitTime();
		CGame::GetInstance()->InitiateSwitchScene(scene_id);
	}
}
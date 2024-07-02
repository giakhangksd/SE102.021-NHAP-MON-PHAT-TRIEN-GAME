#include "Hud.h"
#include "Brick.h"
#include "Goomba.h"
#include "KoopaTroopa.h"
#include "Mario.h"
#include "Game.h"
#include "PlayScene.h"
#include "Mushroom.h"
#include "Coin.h"
#include "PSwitch.h"
#include "debug.h"


void CBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (time_start == -1)
		return;

	vy += ay * dt;

	x += vx * dt;
	y += vy * dt;

	if (type == BRICK_TYPE_EMPTY || state == BRICK_STATE_DEFLECT) {
		if (y > old_y) {
			y = old_y;
			time_start = -1;
			state = -1;
			vy = ay = 0;

			CGameObject* _object = NULL;
			switch (containObject) {
			case BRICK_CONTAIN_SUPER_MUSHROOM_LEAF: {
				CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

				switch (mario->GetLevel()) {
				case MARIO_LEVEL_SMALL:
					_object = new CMushroom(x, y, MUSHROOM_TYPE_SUPER);
					break;

				case MARIO_LEVEL_BIG:
				case MARIO_LEVEL_RACOON:
					_object = new CMushroom(x, y, MUSHROOM_TYPE_SUPER_LEAF);

					break;
				}
				((CMushroom*)_object)->CreatedByBrick();
			}
												  break;

			case BRICK_CONTAIN_COIN:
				_object = new CCoin(x, y);
				_object->Deflected();
				CHud::GetInstance()->CollectCoin();
				break;

			case BRICK_CONTAIN_1UP_MUSHROOM:
				_object = new CMushroom(x, y, MUSHROOM_TYPE_1UP);
				((CMushroom*)_object)->CreatedByBrick();
				break;

			case BRICK_CONTAIN_SWITCH:
				_object = new CPSwitch(x, y - PSWITCH_BBOX_HEIGHT);
				break;
			}

			if (_object) ((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetObjects().push_back(_object);
		}
	}
	else if (type == BRICK_TYPE_BREAK)
		if (GetTickCount64() - time_start > BRICK_BREAK_TIMEOUT) {
			if (bricks.size() > 0) {
				vector<CBrick*>::iterator it;
				for (it = bricks.begin(); it != bricks.end(); it++) {
					CBrick* o = *it;
					if (o->IsDeleted()) {
						delete o;
						*it = NULL;
					}
				}
			}
			this->Delete();
		}

	if (bricks.size() > 0)
		for (int i = 0; i < 3; i++)
			bricks[i]->Update(dt, coObjects);
}

void CBrick::Render()
{
	int aniId = ID_ANI_GOLD_BRICK;
	if (type == BRICK_TYPE_QUESTION)
		aniId = ID_ANI_QUESTION_BRICK;
	else if (type == BRICK_TYPE_EMPTY)
		aniId = ID_ANI_EMPTY_BRICK;
	else if (type == BRICK_TYPE_BREAK)
		aniId = ID_ANI_BREAK_BRICK;

	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(aniId)->Render(x, y);

	if (bricks.size() > 0)
		for (int i = 0; i < 3; i++)
			bricks[i]->Render();
}

void CBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - BRICK_BBOX_WIDTH / 2;
	t = y - BRICK_BBOX_HEIGHT / 2;
	r = l + BRICK_BBOX_WIDTH;
	b = t + BRICK_BBOX_HEIGHT;
}

void CBrick::SetType(int type)
{
	if (containObject != BRICK_CONTAIN_NONE && type == BRICK_TYPE_BREAK) {
		SetType(BRICK_TYPE_EMPTY);
		return;
	}

	switch (type) {
	case BRICK_TYPE_EMPTY:
		time_start = GetTickCount64();
		old_y = y;
		Deflected(0);
		break;

	case BRICK_TYPE_BREAK:
		time_start = GetTickCount64();
		ay = BRICK_GRAVITY;
		vx = -BRICK_BREAK_DEFLECT_X;
		vy = -BRICK_BREAK_DEFLECT_Y;

		if (type == BRICK_TYPE_GOLD) {
			bricks.push_back(new CBrick(x + BRICK_BBOX_WIDTH, y, BRICK_TYPE_BREAK, BRICK_CONTAIN_NONE));
			bricks[0]->SetSpeed(BRICK_BREAK_DEFLECT_X, -BRICK_BREAK_DEFLECT_Y);
			bricks[0]->time_start = GetTickCount64();
			bricks[0]->ay = BRICK_GRAVITY;

			bricks.push_back(new CBrick(x, y + BRICK_BBOX_HEIGHT, BRICK_TYPE_BREAK, BRICK_CONTAIN_NONE));
			bricks[1]->SetSpeed(-BRICK_BREAK_DEFLECT_X, -BRICK_BREAK_DEFLECT_X);
			bricks[1]->time_start = GetTickCount64();
			bricks[1]->ay = BRICK_GRAVITY;

			bricks.push_back(new CBrick(x + BRICK_BBOX_WIDTH, y + BRICK_BBOX_HEIGHT, BRICK_TYPE_BREAK, BRICK_CONTAIN_NONE));
			bricks[2]->SetSpeed(BRICK_BREAK_DEFLECT_X, -BRICK_BREAK_DEFLECT_X);
			bricks[2]->time_start = GetTickCount64();
			bricks[2]->ay = BRICK_GRAVITY;
		}
		break;
	}
	this->type = type;
}

void CBrick::SetState(int State) {
	switch (State)
	{
	case BRICK_STATE_DEFLECT:
		time_start = GetTickCount64();
		old_y = y;
		Deflected(0);
		if (containObject) SetType(BRICK_TYPE_EMPTY);
		break;
	}
	state = State;
}

bool CBrick::IsAttacking() {
	return this != NULL && time_start != -1;
}

void CBrick::BrickTransformCoin() {
	if (type == BRICK_TYPE_GOLD && containObject == BRICK_CONTAIN_NONE
		&& (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene()) {
		this->Delete();

		CGameObject* coin = new CCoin(x, y, COIN_TRANSFORMED_FROM_BRICK);
		((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetObjects().push_back(coin);
	}
}

void CBrick::Deflected(int Direction) {
	ay = BRICK_GRAVITY;
	vy = -BRICK_TRANSFORM_DEFLECT;
	vx = 0;
}

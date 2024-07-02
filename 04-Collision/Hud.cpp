#include "Hud.h"
#include "Game.h"
#include "debug.h"
#include "Timer.h"
#include "Sprites.h"
#include "AssetIDs.h"
#include "Platform.h"
#include "PlayScene.h"
#include "Animations.h"

CHud* CHud::_instance = NULL;

void CHud::RenderNumber(float x, float y, int number, int length, int option) {
	CSprites* sprites = CSprites::GetInstance();

	switch (option)
	{
	case 1: // Render score and time
		for (int i = 0; i < length; i++) {
			int digit = number % 10;
			sprites->Get(ID_SPRITE_NUMBER_0 + digit)->Draw(x, y);
			x -= NUMBER_WIDTH;
			number /= 10;
		}
		break;

	case 0: // Render number
		if (number == 0) sprites->Get(ID_SPRITE_NUMBER_0)->Draw(x, y);

		while (number > 0) {
			int digit = number % 10;
			sprites->Get(ID_SPRITE_NUMBER_0 + digit)->Draw(x, y);
			x -= NUMBER_WIDTH;
			number /= 10;
		}
		break;
	}
}

void CHud::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = x - HUD_BBOX_WIDTH / 2;
	top = y - HUD_BBOX_HEIGHT / 2;
	right = left + HUD_BBOX_WIDTH;
	bottom = top + HUD_BBOX_HEIGHT;
}

void CHud::Update() {
	// Get camera position
	float cam_x, cam_y;
	CGame::GetInstance()->GetCamPos(cam_x, cam_y);

	// Set hud position follows camera when it moves
	x = cam_x + HUD_BBOX_WIDTH * 3 / 4 - 4;
	y = (cam_y + CGame::GetInstance()->GetBackBufferHeight() * 4 / 5 + HUD_BBOX_HEIGHT - ADJUST);
}

void CHud::Render() {
	Update(); // Update
	CSprites* sprites = CSprites::GetInstance();

	// Draw black background
	CPlatform* platform1 = new CPlatform(x - HUD_BBOX_WIDTH * 2 / 3, y - HUD_BBOX_HEIGHT / 8 - 4, 16, 16, 20, 55631, 55631, 55631, 3);
	CPlatform* platform2 = new CPlatform(x - HUD_BBOX_WIDTH * 2 / 3, y - HUD_BBOX_HEIGHT / 8 + 12, 16, 16, 20, 55631, 55631, 55631, 3);

	platform1->Render();
	platform2->Render();

	// Draw hud
	sprites->Get(ID_SPRITE_HUD)->Draw(x, y);
	//DebugOut(L"Hud\n");

	//Draw giftbox and gift (if have)
	float giftBox_x, giftBox_y;
	giftBox_x = x + HUD_BBOX_WIDTH / 2 + GIFTBOX_BBOX_WIDTH;
	giftBox_y = y;

	for (int i = 0; i < 3; i++) {
		sprites->Get(ID_SPRITE_GIFT_BOX_BLUE)->Draw(giftBox_x, giftBox_y);

		if (gifts.size() > i)
			sprites->Get(gifts[i])->Draw(giftBox_x, giftBox_y);

		giftBox_x += GIFTBOX_BBOX_WIDTH;
	}

	// Get left top corner of hud
	float hx, hy;
	hx = x - HUD_BBOX_WIDTH / 2;
	hy = y - HUD_BBOX_HEIGHT / 2;

	// Draw name of character Mario or Luigi (M or L)
	sprites->Get(ID_SPRITE_SYMBOL_M)->Draw(hx + MARIO_SYMBOL_WIDTH / 2 + COLUMN_ML_X, hy + MARIO_SYMBOL_HEIGHT / 2 + ROW_BELOW_Y);

	// Draw name of current world
	sprites->Get(ID_SPRITE_NUMBER_1)->Draw(hx - NUMBER_WIDTH / 2 + COLUMN_WL_X, hy + ROW_ABOVE_Y + NUMBER_HEIGHT / 2);

	// Draw life
	float p_x = hx - NUMBER_WIDTH / 2 + COLUMN_WL_X;
	float p_y = hy + ROW_BELOW_Y + NUMBER_HEIGHT / 2;
	RenderNumber(p_x, p_y, life);

	// Draw score
	p_x = hx - NUMBER_WIDTH / 2 + COLUMN_SCORE_X;
	p_y = hy + ROW_BELOW_Y + NUMBER_HEIGHT / 2;
	RenderNumber(p_x, p_y, score, LENGTH_SCORE, 1);

	// Draw coin
	p_x = hx - NUMBER_WIDTH / 2 + COLUMN_MT_X;
	p_y = hy + ROW_ABOVE_Y + NUMBER_HEIGHT / 2;
	RenderNumber(p_x, p_y, coin);

	// Draw time
	p_x = hx - NUMBER_WIDTH / 2 + COLUMN_MT_X;
	p_y = hy + ROW_BELOW_Y + NUMBER_HEIGHT / 2;
	RenderNumber(p_x, p_y, (int) CTimer::GetInstance()->getCurrentTime() / 1000, LENGTH_TIME, 1);

	// Draw Mario's speed state (P speed)
	p_x = hx - NUMBER_WIDTH / 2 + COLUMN_SPEED_X;
	p_y = hy + ROW_ABOVE_Y + NUMBER_HEIGHT / 2;

	// Get speed of Mario
	float mario_vx = 0;
	float mario_vy = 0;
	bool isFlying = false;

	if (dynamic_cast<LPPLAYSCENE>(CGame::GetInstance()->GetCurrentScene())) {
		CMario* mario = dynamic_cast<CMario*>(((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer());
		mario->GetSpeed(mario_vx, mario_vy);
		isFlying = mario->IsFlying();
	}

	// Render Mario's speed state with 6 level
	for (int i = 0; i < 6; i++) {
		if ((abs(mario_vx) - MARIO_WALKING_SPEED) >= (SPEED_LEVEL * (i + 1)) || isFlying)
			sprites->Get(ID_SPRITE_ARROW_WHITE)->Draw(p_x + ARROW_SYMBOL_WIDTH, p_y);
		else sprites->Get(ID_SPRITE_ARROW_BLACK)->Draw(p_x + ARROW_SYMBOL_WIDTH, p_y);
		p_x += ARROW_SYMBOL_WIDTH + 1;
	}

	// If reach running speed or is flying, power icon will changing color repeatly
	if (abs(mario_vx) == MARIO_RUNNING_SPEED || isFlying)
		CAnimations::GetInstance()->Get(ID_ANI_P_CHANGING_COLOR)->Render(p_x + POWER_SYMBOL_WIDTH, p_y);
	else sprites->Get(ID_SPRITE_P_BLACK)->Draw(p_x + POWER_SYMBOL_WIDTH, p_y);
}
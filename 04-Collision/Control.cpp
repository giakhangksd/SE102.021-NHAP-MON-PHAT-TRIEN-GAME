#include "Game.h"
#include "Control.h"
#include "PlayScene.h"

CControl* CControl::_instance = NULL;

void CControl::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = x - CONTROL_PAUSE_BBOX_WIDTH / 2;
	top = y - CONTROL_PAUSE_BBOX_HEIGHT / 2;
	right = left + CONTROL_PAUSE_BBOX_WIDTH;
	bottom = top + CONTROL_PAUSE_BBOX_HEIGHT;
}

void CControl::CalculatePosition(int type) {
	float cam_x, cam_y;
	CGame::GetInstance()->GetCamPos(cam_x, cam_y);

	switch (type) {
	case CONTROL_TYPE_PAUSE:
		x = cam_x + CGame::GetInstance()->GetBackBufferWidth() / 2;
		y = (cam_y + CGame::GetInstance()->GetBackBufferHeight() / 2 + CONTROL_PAUSE_BBOX_HEIGHT / 100);
		break;

	case CONTROL_TYPE_MODE_ONE:
	case CONTROL_TYPE_MODE_TWO:
		x = cam_x + CGame::GetInstance()->GetBackBufferWidth() / 2;
		y = (cam_y + CGame::GetInstance()->GetBackBufferHeight() * 2 / 3 + CONTROL_MODE_BBOX_HEIGHT + 15);
		break;
	}
}

void CControl::Render() {
	CSprites* sprites = CSprites::GetInstance();
	for (int i = 0; i < n; i++) {
		if (active[i] == 0)
			continue;

		this->CalculatePosition(i + 1);

		int spriteId = -1;
		switch (i + 1) { // type
		case CONTROL_TYPE_PAUSE:
			spriteId = ID_SPRITE_PAUSE;
			break;

		case CONTROL_TYPE_MODE_ONE:
			spriteId = ID_SPRITE_1PLAYERGAME;
			break;

		case CONTROL_TYPE_MODE_TWO:
			spriteId = ID_SPRITE_2PLAYERGAME;
			break;
		}

		sprites->Get(spriteId)->Draw(x, y);
	}
}
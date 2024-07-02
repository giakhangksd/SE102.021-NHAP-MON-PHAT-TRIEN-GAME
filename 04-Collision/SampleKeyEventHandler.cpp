#include "SampleKeyEventHandler.h"

#include "debug.h"
#include "Game.h"

#include "Mario.h"
#include "Control.h"
#include "PlayScene.h"

//extern CMario* mario;
//extern void Reload();
void CSampleKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	LPGAME game = CGame::GetInstance();
	LPPLAYSCENE scene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	switch (KeyCode)
	{
	case DIK_UP:
		mario->SetCanGetIntoPipe(MARIO_GETINTO_PIPE_UP);
		break;
	case DIK_DOWN:
		if (!mario->IsHolding() && !game->IsKeyDown(DIK_LEFT) && !game->IsKeyDown(DIK_RIGHT))
			mario->SetState(MARIO_STATE_SIT);
		break;
	case DIK_S:
		if (mario->IsOnPlatform()) mario->SetState(MARIO_STATE_JUMP);
		else mario->SetState(MARIO_STATE_FLY);
		break;
	case DIK_1:
		mario->SetLevel(MARIO_LEVEL_SMALL);
		break;
	case DIK_2:
		mario->SetLevel(MARIO_LEVEL_BIG);
		break;
	case DIK_3:
		mario->SetLevel(MARIO_LEVEL_RACOON);
		break;
	case DIK_A:
		if (mario->GetLevel() == MARIO_LEVEL_RACOON) mario->SetState(MARIO_STATE_ATTACK);
		break;
	case DIK_0:
		mario->SetState(MARIO_STATE_DIE);
		break;
	case DIK_R: // reset
		scene->Unload();
		scene->Load();
		break;
	case DIK_ESCAPE:
		if (CControl::GetInstance()->IsPausing()) CControl::GetInstance()->DeactiveControl(CONTROL_TYPE_PAUSE);
		else CControl::GetInstance()->ActiveControl(CONTROL_TYPE_PAUSE);

		break;
	}
}

void CSampleKeyHandler::OnKeyUp(int KeyCode)
{
	//DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	switch (KeyCode)
	{
	case DIK_A:
		mario->SetState(MARIO_STATE_HOLD_RELEASE);
		break;
	case DIK_S:
		if (mario->IsFlying() || mario->IsFloating()) mario->SetState(MARIO_STATE_RELEASE_FLY);
		else mario->SetState(MARIO_STATE_RELEASE_JUMP);
		break;
	case DIK_DOWN:
		mario->SetState(MARIO_STATE_SIT_RELEASE);
		break;
	case DIK_UP:
		mario->SetCanGetIntoPipe(0);
		break;
	}
}

void CSampleKeyHandler::KeyState(BYTE *states)
{
	LPGAME game = CGame::GetInstance();
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	if (game->IsKeyDown(DIK_A))
		mario->SetState(MARIO_STATE_HOLD);

	if (game->IsKeyDown(DIK_RIGHT)) {
		if (game->IsKeyDown(DIK_A) && !mario->IsFlying() && !mario->IsFloating())
			mario->SetState(MARIO_STATE_RUNNING_RIGHT);
		else mario->SetState(MARIO_STATE_WALKING_RIGHT);
	}
	else if (game->IsKeyDown(DIK_LEFT)) {
		if (game->IsKeyDown(DIK_A) && !mario->IsFlying() && !mario->IsFloating())
			mario->SetState(MARIO_STATE_RUNNING_LEFT);
		else mario->SetState(MARIO_STATE_WALKING_LEFT);
	}
	else
		mario->SetState(MARIO_STATE_IDLE);
}
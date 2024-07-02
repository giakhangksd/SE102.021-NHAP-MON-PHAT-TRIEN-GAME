#include "WorldmapKeyEventHandler.h"

#include "debug.h"
#include "Game.h"

#include "MarioWorldmap.h"
#include "Control.h"
#include "WorldmapScene.h"

//extern CMario* mario;
//extern void Reload();
void CWorldmapKeyEventHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	CMarioWorldmap* mario = (CMarioWorldmap*)((LPWORLDMAPSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	LPGAME game = CGame::GetInstance();


	switch (KeyCode)
	{
	case DIK_S:
		mario->SetCanGoIntoPortal(1);
		break;
	case DIK_ESCAPE:
		if (CControl::GetInstance()->IsPausing()) CControl::GetInstance()->DeactiveControl(CONTROL_TYPE_PAUSE);
		else CControl::GetInstance()->ActiveControl(CONTROL_TYPE_PAUSE);
		break;
	}
}

void CWorldmapKeyEventHandler::OnKeyUp(int KeyCode)
{
	//DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
	CMarioWorldmap* mario = (CMarioWorldmap*)((LPWORLDMAPSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	switch (KeyCode)
	{
	case DIK_S:
		mario->SetCanGoIntoPortal(0);
		break;
	}
}

void CWorldmapKeyEventHandler::KeyState(BYTE* states)
{
	CMarioWorldmap* mario = (CMarioWorldmap*)((LPWORLDMAPSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	LPGAME game = CGame::GetInstance();
	if (mario->IsMoving()) return;

	if (game->IsKeyDown(DIK_RIGHT))
		mario->SetState(MARIO_WORLDMAP_STATE_WALKING_RIGHT);
	else if (game->IsKeyDown(DIK_LEFT))
		mario->SetState(MARIO_WORLDMAP_STATE_WALKING_LEFT);
	else if (game->IsKeyDown(DIK_UP))
		mario->SetState(MARIO_WORLDMAP_STATE_WALKING_UP);
	else if (game->IsKeyDown(DIK_DOWN))
		mario->SetState(MARIO_WORLDMAP_STATE_WALKING_DOWN);
}
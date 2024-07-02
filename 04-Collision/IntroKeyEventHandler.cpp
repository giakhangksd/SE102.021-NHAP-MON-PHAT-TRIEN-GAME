#include "Game.h"
#include "debug.h"
#include "Portal.h"
#include "Control.h"
#include "IntroScene.h"
#include "IntroKeyEventHandler.h"

void CIntroKeyEventHandler::OnKeyDown(int KeyCode) {
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	CControl* control = CControl::GetInstance();
	LPINTROSCENE scene = (LPINTROSCENE)CGame::GetInstance()->GetCurrentScene();

	switch (KeyCode) {
	case DIK_S:
		if (!control->IsActivatingModeTwo()) {
			if (control->IsActivatingModeOne()) { // switch to WorldMap
				control->DeactiveControl(CONTROL_TYPE_MODE_ONE);

				DebugOut(L"Switch scene\n");

				CHud::GetInstance()->ResetScore();
				((LPINTROSCENE)CGame::GetInstance()->GetCurrentScene())->GetPortal()->SwitchScene();
			}
			else control->ActiveControl(CONTROL_TYPE_MODE_ONE);
		}

		break;

	case DIK_UP:
		if (control->IsActivatingModeTwo()) {
			control->DeactiveControl(CONTROL_TYPE_MODE_TWO);
			control->ActiveControl(CONTROL_TYPE_MODE_ONE);
		}
		break;

	case DIK_DOWN:
		if (control->IsActivatingModeOne()) {
			control->DeactiveControl(CONTROL_TYPE_MODE_ONE);
			control->ActiveControl(CONTROL_TYPE_MODE_TWO);
		}
		break;

	case DIK_ESCAPE:
		if (control->IsPausing()) control->DeactiveControl(CONTROL_TYPE_PAUSE);
		else control->ActiveControl(CONTROL_TYPE_PAUSE);
		break;

	case DIK_R: // reset
		scene->Unload();
		scene->Load();
		break;
	}
}

void CIntroKeyEventHandler::OnKeyUp(int KeyCode) {
	//DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
}

void CIntroKeyEventHandler::KeyState(BYTE* states) {
	LPGAME game = CGame::GetInstance();

	float cx, cy;
	game->GetCamPos(cx, cy);
	if (game->IsKeyDown(DIK_RIGHT))
		game->SetCamPos(cx + 1, cy);
	else if (game->IsKeyDown(DIK_LEFT))
		game->SetCamPos(cx - 1, cy);
}
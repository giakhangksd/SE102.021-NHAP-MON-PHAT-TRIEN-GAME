#pragma once

#include "Scene.h"

class CIntroKeyEventHandler : public CSceneKeyHandler {
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CIntroKeyEventHandler(LPSCENE s) :CSceneKeyHandler(s) {};
};
#pragma once

#include "Scene.h"

class CWorldmapKeyEventHandler : public CSceneKeyHandler {
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CWorldmapKeyEventHandler(LPSCENE s) : CSceneKeyHandler(s) {};
};
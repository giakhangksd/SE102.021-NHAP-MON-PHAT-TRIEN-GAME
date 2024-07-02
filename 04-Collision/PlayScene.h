#pragma once
#include "Portal.h"
#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "GameObject.h"
#include "Brick.h"
#include "Mario.h"
#include "Goomba.h"
#include "Mushroom.h"
#include "PiranhaPlant.h"
#include "InvisibleObject.h"

#define CAMERA_POSITION_MIN_Y			0
#define CAMERA_POSITION_MAX_X			2499
#define DEFAULT_CAMERA_POSITION_Y		252 //223
#define POSITION_SECRET_ROOM			2710
#define POSITION_SECRET_ROOM_Y			220

class CPlayScene : public CScene
{
protected:
	// A play scene has to have player, right? 
	LPGAMEOBJECT player;

	// InvisibleObject
	CInvisibleObject* invisible;

	// Switch scene when Mario DIE
	CPortal* portal;

	vector<LPGAMEOBJECT> objects;

	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);

	void _ParseSection_ASSETS(string line);
	void _ParseSection_OBJECTS(string line);

	void LoadAssets(LPCWSTR assetFile);

public:
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	LPGAMEOBJECT GetPlayer() { return player; }
	vector<LPGAMEOBJECT>& GetObjects() { return objects; }
	CInvisibleObject* GetInvisibleObject() { return invisible; }

	void Clear();
	void PurgeDeletedObjects();

	static bool IsGameObjectDeleted(const LPGAMEOBJECT& o);

	CPortal* GetPortal() { return portal; }
};

typedef CPlayScene* LPPLAYSCENE;


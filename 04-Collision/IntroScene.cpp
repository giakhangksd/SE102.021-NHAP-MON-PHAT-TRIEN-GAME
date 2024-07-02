#include <fstream>
#include <iostream>

#include "Hud.h"
#include "Coin.h"
#include "Utils.h"
#include "Portal.h"
#include "Sprites.h"
#include "AssetIDs.h"
#include "Platform.h"
#include "Textures.h"
#include "IntroScene.h"
#include "TeleportGate.h"
#include "PlatformAnimate.h"
#include "IntroKeyEventHandler.h"

using namespace std;

CIntroScene::CIntroScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	player[0] = NULL;
	player[1] = NULL;
	flag = 0;
	key_handler = new CIntroKeyEventHandler(this);
}


#define SCENE_SECTION_UNKNOWN		-1
#define SCENE_SECTION_ASSETS		1
#define SCENE_SECTION_OBJECTS		2

#define ASSETS_SECTION_UNKNOWN		-1
#define ASSETS_SECTION_SPRITES		1
#define ASSETS_SECTION_ANIMATIONS	2

#define MAX_SCENE_LINE				1024

void CIntroScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPTEXTURE tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CIntroScene::_ParseSection_ASSETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return;

	wstring path = ToWSTR(tokens[0]);

	LoadAssets(path.c_str());
}

void CIntroScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

/*
	Parse a line in section [OBJECTS]
*/
void CIntroScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	// skip invalid lines - an object set must have at least id, x, y
	if (tokens.size() < 2) return;

	int object_type = atoi(tokens[0].c_str());
	float x = (float)atof(tokens[1].c_str());
	float y = (float)atof(tokens[2].c_str());

	CGameObject* obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_MARIO: {
		int type = (int)atof(tokens[3].c_str());

		if (player[0] != NULL && player[1] != NULL) {
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}

		obj = new CMario(x, y, type);
		obj->SetState(MARIO_STATE_IDLE);
		if (player[0] == NULL) {
			player[0] = (CMario*)obj;
			((CMario*)player[0])->SetLevel(MARIO_LEVEL_BIG);
		}
		else {
			player[1] = (CMario*)obj;
			((CMario*)player[1])->SetLevel(MARIO_LEVEL_BIG);
		}

		DebugOut(L"[INFO] Player object has been created!\n");

		break;
	}

	case OBJECT_TYPE_PLATFORM_ANIMATE: { // Only for number 3
		int aniOrsprite = atoi(tokens[3].c_str());
		int type = atoi(tokens[4].c_str());
		int isAni = atoi(tokens[5].c_str());

		obj = new CPlatformAnimate(x, y, aniOrsprite, type, isAni);
		Number3 = (CPlatformAnimate*)(obj);

		Number3Original_y = y;
		break;
	}

	case OBJECT_TYPE_PLATFORM: { // Another like curtain, cloud,...

		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int sprite_begin = atoi(tokens[6].c_str());
		int sprite_middle = atoi(tokens[7].c_str());
		int sprite_end = atoi(tokens[8].c_str());
		int type = atoi(tokens[9].c_str());
		obj = new CPlatform(
			x, y,
			cell_width, cell_height, length,
			sprite_begin, sprite_middle, sprite_end,
			type
		);

		platformObjects.push_back((CPlatform*)obj);

		break;
	}

	case OBJECT_TYPE_PORTAL: { // Portal to select mode
		float r = (float)atof(tokens[3].c_str());
		float b = (float)atof(tokens[4].c_str());
		int scene_id = atoi(tokens[5].c_str());
		int type = atoi(tokens[6].c_str());
		obj = new CPortal(x, y, r, b, scene_id, type);

		portal = (CPortal*)obj;


		break;
	}


	default:
		DebugOut(L"[ERROR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);


	objects.push_back(obj);
}

void CIntroScene::LoadAssets(LPCWSTR assetFile)
{
	DebugOut(L"[INFO] Start loading assets from : %s \n", assetFile);

	ifstream f;
	f.open(assetFile);

	int section = ASSETS_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[SPRITES]") { section = ASSETS_SECTION_SPRITES; continue; };
		if (line == "[ANIMATIONS]") { section = ASSETS_SECTION_ANIMATIONS; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case ASSETS_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case ASSETS_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading assets from %s\n", assetFile);
}

void CIntroScene::Load()
{
	DebugOut(L"[INFO] Start loading scene from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	
		if (line == "[ASSETS]") { section = SCENE_SECTION_ASSETS; continue; };
		if (line == "[OBJECTS]") { section = SCENE_SECTION_OBJECTS; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_ASSETS: _ParseSection_ASSETS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}

	f.close();

	// Start intro with curtain flying
	flag = CURTAIN_FLYING;
	timeStart = GetTickCount64();
	DebugOut(L"[INFO] Done loading scene  %s\n", sceneFilePath);
}

void CIntroScene::Update(DWORD dt)
{
	// This make intro exactly
	if (MarioWalkingStart != 0)
		MarioWalkingStart += (dt - 15);
	if (timeStart != 0)
		timeStart += (dt - 15);
	dt = 15;

	// PAUSING
	if (CControl::GetInstance()->IsPausing())
	{
		timeStart += dt;
		return;
	}

	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 
	if (!((CMario*)player[1])->IsTransforming()) {

		vector<LPGAMEOBJECT> coObjects;

		// Update platform objects first
		for (size_t i = 0; i < platformObjects.size(); i++)
			coObjects.push_back(objects[i]);

		// Then item and enemies
		for (int i = 0; i < ItemsAndEnemies.size(); i++)
			coObjects.push_back(ItemsAndEnemies[i]);

		player[0]->Update(dt, &coObjects);
		player[1]->Update(dt, &coObjects);

		// Update items and enemies
		for (int i = 0; i < ItemsAndEnemies.size(); i++)
			ItemsAndEnemies[i]->Update(dt, &coObjects);

		// Introduction of Super Mario Bros 3
		// Update another things
		Action(dt);
	}

	// Deleting objects out of Screen
	float x, y;
	int Screen_Height = CGame::GetInstance()->GetBackBufferHeight();
	int Screen_Width = CGame::GetInstance()->GetBackBufferWidth();

	for (auto e : ItemsAndEnemies) {
		e->GetPosition(x, y);
		if (y > Screen_Height + 20 || x < -100 || x > Screen_Width + 70)
			e->Delete();
	}

	PurgeDeletedObjects();
}

void CIntroScene::Render()
{
	if (flag == CURTAIN_FLYING) {
		player[0]->Render();
		player[1]->Render();

		// Black screen
		platformObjects[1]->Render();
		platformObjects[2]->Render();
	}

	for (size_t i = platformObjects.size() - 1; i > 2; i--)
		platformObjects[i]->Render();

	if (MarioWalkingStart != 0 && flag == 0) {
		platformObjects[1]->Render();
		platformObjects[2]->Render();

		player[0]->Render();
		player[1]->Render();
	}

	if (flag != CURTAIN_FLYING) {
		Number3->Render();
		platformObjects[2]->Render();
		player[0]->Render();
		player[1]->Render();

		for (int i = 0; i < ItemsAndEnemies.size(); i++)
			ItemsAndEnemies[i]->Render();
	}

	// Render stage
	platformObjects[0]->Render();

	CControl::GetInstance()->Render();
}

/*
*	Clear all objects from this scene
*/
void CIntroScene::Clear()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		delete (*it);
	}
	objects.clear();
}

/*
	Unload scene

	TODO: Beside objects, we need to clean up sprites, animations and textures as well

*/
void CIntroScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player[0] = NULL;
	player[1] = NULL;
	platformObjects.clear();
	ItemsAndEnemies.clear();
	Number3 = NULL;

	DebugOut(L"[INFO] Scene %d unloaded! \n", id);
}

bool CIntroScene::IsGameObjectDeleted(const LPGAMEOBJECT& o) { return o == NULL; }

void CIntroScene::PurgeDeletedObjects()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		LPGAMEOBJECT o = *it;
		if (o->IsDeleted())
		{
			delete o;
			*it = NULL;
		}
	}

	for (it = ItemsAndEnemies.begin(); it != ItemsAndEnemies.end(); it++)
	{
		LPGAMEOBJECT o = *it;
		if (o->IsDeleted())
		{
			delete o;
			*it = NULL;
		}
	}

	// remove ItemsAndEnemies
	ItemsAndEnemies.erase(
		std::remove_if(ItemsAndEnemies.begin(), ItemsAndEnemies.end(), CIntroScene::IsGameObjectDeleted),
		ItemsAndEnemies.end());

	// NOTE: remove_if will swap all deleted items to the end of the vector
	// then simply trim the vector, this is much more efficient than deleting individual items
	objects.erase(
		std::remove_if(objects.begin(), objects.end(), CIntroScene::IsGameObjectDeleted),
		objects.end());

}
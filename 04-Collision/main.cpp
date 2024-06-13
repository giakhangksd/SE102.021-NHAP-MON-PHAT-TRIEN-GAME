/* =============================================================
	INTRODUCTION TO GAME PROGRAMMING SE102
	
	SAMPLE 04 - COLLISION

	This sample illustrates how to:

		1/ Implement SweptAABB algorithm between moving objects
		2/ Implement a simple (yet effective) collision frame work, applying on Mario, Brick, Goomba & Coin

	Key functions: 
		CCollision::SweptAABB
		CCollision::SweptAABBEx
		CCollision::Scan
		CCollision::Filter
		CCollision::Process

		CGameObject::GetBoundingBox
		
================================================================ */

#include <windows.h>
#include <d3d10.h>
#include <d3dx10.h>
#include <list>

#include "debug.h"
#include "Game.h"
#include "GameObject.h"
#include "Textures.h"
#include "Animation.h"
#include "Animations.h"

#include "Mario.h"
#include "Brick.h"
#include "Goomba.h"
#include "Coin.h"
#include "Platform.h"
#include "pipe.h"
#include "underpipe.h"
#include "quesbox.h"
#include "Mushroom.h"
#include "leaf.h"
#include "coineffect.h"
#include "worldmap.h"
#include "Koopa.h"
#include "piranhaplant.h"

#include "SampleKeyEventHandler.h"

#include "AssetIDs.h"

#define WINDOW_CLASS_NAME L"SampleWindow"
#define MAIN_WINDOW_TITLE L"04 - Collision"
#define WINDOW_ICON_PATH L"mario.ico"

#define BACKGROUND_COLOR D3DXCOLOR(200.0f/255, 200.0f/255, 255.0f/255, 0.0f)

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#define TEXTURES_DIR L"textures"
#define TEXTURE_PATH_MARIO TEXTURES_DIR "\\mario_transparent.png"
#define TEXTURE_PATH_MISC TEXTURES_DIR "\\misc_transparent.png"
#define TEXTURE_PATH_ENEMY TEXTURES_DIR "\\enemies_transparent.png"
#define TEXTURE_PATH_BBOX TEXTURES_DIR "\\bbox.png"
#define TEXTURE_PATH_MAP TEXTURES_DIR "\\map.png"

CGame *game;
CMario *mario;

list<LPGAMEOBJECT> objects;

CSampleKeyHandler * keyHandler; 

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

void LoadAssetsMario()
{
	CTextures* textures = CTextures::GetInstance();
	CSprites* sprites = CSprites::GetInstance();
	CAnimations* animations = CAnimations::GetInstance();

	LPTEXTURE texMario = textures->Get(ID_TEX_MARIO);

	// IDLE
	sprites->Add(ID_SPRITE_MARIO_BIG_IDLE_RIGHT + 1, 246,154,259,181, texMario);
	sprites->Add(ID_SPRITE_MARIO_BIG_IDLE_LEFT + 1, 186,154,199,181, texMario);

	// WALKING LEFT
	sprites->Add(ID_SPRITE_MARIO_BIG_WALKING_RIGHT + 2, 275,154,290,181, texMario);
	sprites->Add(ID_SPRITE_MARIO_BIG_WALKING_RIGHT + 3, 304,154,321,181, texMario);

	// WALKING RIGHT
	sprites->Add(ID_SPRITE_MARIO_BIG_WALKING_LEFT + 2, 155,154,170,181, texMario);
	sprites->Add(ID_SPRITE_MARIO_BIG_WALKING_LEFT + 3, 125,154,140,181, texMario);

	// RUNNING RIGHT 
	sprites->Add(ID_SPRITE_MARIO_BIG_RUNNING_RIGHT + 1, 334,154,355,181, texMario);
	sprites->Add(ID_SPRITE_MARIO_BIG_RUNNING_RIGHT + 2, 334,154,355,181, texMario);
	sprites->Add(ID_SPRITE_MARIO_BIG_RUNNING_RIGHT + 3, 392,154,413,181, texMario);

	// RUNNING LEFT
	sprites->Add(ID_SPRITE_MARIO_BIG_RUNNING_LEFT + 1, 91,154,112,181, texMario);
	sprites->Add(ID_SPRITE_MARIO_BIG_RUNNING_LEFT + 2, 65,154,86,181, texMario);
	sprites->Add(ID_SPRITE_MARIO_BIG_RUNNING_LEFT + 3, 34,154,55,181, texMario);

	// JUMP WALK RIGHT & LEFT 
	sprites->Add(ID_SPRITE_MARIO_BIG_JUMP_WALK_RIGHT + 1, 395,275,412,302, texMario);
	sprites->Add(ID_SPRITE_MARIO_BIG_JUMP_WALK_LEFT + 1, 35,275,52,302, texMario);

	// JUMP RUN RIGHT & LEFT 
	sprites->Add(ID_SPRITE_MARIO_BIG_JUMP_RUN_RIGHT + 1, 394,195,413,222, texMario);
	sprites->Add(ID_SPRITE_MARIO_BIG_JUMP_RUN_LEFT + 1, 35,195,52,222, texMario);

	// SIT RIGHT/LEFT
	sprites->Add(ID_SPRITE_MARIO_BIG_SIT_RIGHT + 1, 426,239,441,256, texMario);
	sprites->Add(ID_SPRITE_MARIO_BIG_SIT_LEFT + 1, 5,239,20,256, texMario);

	// BRACING RIGHT/LEFT
	sprites->Add(ID_SPRITE_MARIO_BIG_BRACE_RIGHT + 1, 425,154,442,181, texMario);
	sprites->Add(ID_SPRITE_MARIO_BIG_BRACE_LEFT + 1, 5,154,22,181, texMario);

	////////////////// FOX_MARIO //////////////
		// IDLE
	sprites->Add(ID_SPRITE_MARIO_FOX_IDLE_RIGHT + 1, 246-5, 154+480, 259+9, 181+480, texMario);
	sprites->Add(ID_SPRITE_MARIO_FOX_IDLE_LEFT + 1, 186-9, 154+480, 199+5, 181+480, texMario);

	// WALKING LEFT
	sprites->Add(ID_SPRITE_MARIO_FOX_WALKING_RIGHT + 2, 275-5, 154+480, 290+5, 181+480, texMario);
	sprites->Add(ID_SPRITE_MARIO_FOX_WALKING_RIGHT + 3, 304-5, 154+480, 321+5, 181+480, texMario);

	// WALKING RIGHT
	sprites->Add(ID_SPRITE_MARIO_FOX_WALKING_LEFT + 2, 155-5, 154+480, 170+5, 181+480, texMario);
	sprites->Add(ID_SPRITE_MARIO_FOX_WALKING_LEFT + 3, 125-5, 154+480, 140+5, 181+480, texMario);

	// RUNNING RIGHT 
	sprites->Add(ID_SPRITE_MARIO_FOX_RUNNING_RIGHT + 1, 334-5, 154+480, 355+5, 181+480, texMario);
	sprites->Add(ID_SPRITE_MARIO_FOX_RUNNING_RIGHT + 2, 334-5, 154+480, 355+5, 181+480, texMario);
	sprites->Add(ID_SPRITE_MARIO_FOX_RUNNING_RIGHT + 3, 392-5, 154+480, 413+5, 181+480, texMario);

	// RUNNING LEFT
	sprites->Add(ID_SPRITE_MARIO_FOX_RUNNING_LEFT + 1, 91-7, 154+480, 112+3, 181+480, texMario);
	sprites->Add(ID_SPRITE_MARIO_FOX_RUNNING_LEFT + 2, 65-7, 154+480, 86+3, 181+480, texMario);
	sprites->Add(ID_SPRITE_MARIO_FOX_RUNNING_LEFT + 3, 34-7, 154+480, 55+3, 181+480, texMario);

	// JUMP WALK RIGHT & LEFT 
	sprites->Add(ID_SPRITE_MARIO_FOX_JUMP_WALK_RIGHT + 1, 395-5, 275+478, 412+5, 302+478, texMario);
	sprites->Add(ID_SPRITE_MARIO_FOX_JUMP_WALK_LEFT + 1, 35-5, 275+478, 52+5, 302+478, texMario);

	// JUMP RUN RIGHT & LEFT 
	sprites->Add(ID_SPRITE_MARIO_FOX_JUMP_RUN_RIGHT + 1, 394-33, 195+478+80, 413-29, 222+478+80, texMario);
	sprites->Add(ID_SPRITE_MARIO_FOX_JUMP_RUN_LEFT + 1, 35+26, 195+478+80, 52+32, 222+478+80, texMario);

	// SIT RIGHT/LEFT
	sprites->Add(ID_SPRITE_MARIO_FOX_SIT_RIGHT + 1, 425 - 5, 154 + 480+80, 442 + 5, 181 + 480+80, texMario);
	sprites->Add(ID_SPRITE_MARIO_FOX_SIT_LEFT + 1, 5 - 5, 154 + 480+80, 22 + 5, 181 + 480+80, texMario);

	// BRACING RIGHT/LEFT
	sprites->Add(ID_SPRITE_MARIO_FOX_BRACE_RIGHT + 1, 425-5, 154+480, 442+5, 181+480, texMario);
	sprites->Add(ID_SPRITE_MARIO_FOX_BRACE_LEFT + 1, 5-5, 154+480, 22+5, 181+480, texMario);

	// FOX TAIL HIT RIGHT
	sprites->Add(ID_SPRITE_MARIO_FOX_HIT_RIGHT + 1, 246 - 5 - 2, 154 + 480, 259 + 9 - 2, 181 + 480, texMario);
	sprites->Add(ID_SPRITE_MARIO_FOX_HIT_RIGHT + 2, 216 - 9 - 2, 154 + 480, 239 + 5 - 2, 181 + 480, texMario);
	sprites->Add(ID_SPRITE_MARIO_FOX_HIT_RIGHT + 3, 186 - 9 - 2, 154 + 480, 199 + 5 - 2, 181 + 480, texMario);

	// FOX TAIL HIT LEFT
	sprites->Add(ID_SPRITE_MARIO_FOX_HIT_LEFT + 1, 186 - 9 - 2, 154 + 480, 199 + 5 - 2, 181 + 480, texMario);
	sprites->Add(ID_SPRITE_MARIO_FOX_HIT_LEFT + 2, 216 - 9 - 2, 154 + 480, 239 + 5 - 2, 181 + 480, texMario);
	sprites->Add(ID_SPRITE_MARIO_FOX_HIT_LEFT + 3, 246 - 5 - 2, 154 + 480, 259 + 9 - 2, 181 + 480, texMario);

	// MARIO DIE
	sprites->Add(ID_SPRITE_MARIO_DIE + 1, 215, 120, 231, 135, texMario);

	// SMALL MARIO 
	sprites->Add(ID_SPRITE_MARIO_SMALL_IDLE_RIGHT + 1, 247, 0, 259, 15, texMario);			
	sprites->Add(ID_SPRITE_MARIO_SMALL_IDLE_LEFT + 1, 187, 0, 198, 15, texMario);			

	sprites->Add(ID_SPRITE_MARIO_SMALL_WALKING_RIGHT + 2, 275, 0, 291, 15, texMario);			
	sprites->Add(ID_SPRITE_MARIO_SMALL_WALKING_RIGHT + 3, 306, 0, 320, 15, texMario); 

	sprites->Add(ID_SPRITE_MARIO_SMALL_WALKING_LEFT + 2, 155, 0, 170, 15, texMario);
	sprites->Add(ID_SPRITE_MARIO_SMALL_WALKING_LEFT + 3, 125, 0, 139, 15, texMario);

	sprites->Add(ID_SPRITE_MARIO_SMALL_RUNNING_RIGHT + 1, 275, 0, 275 + 15, 15, texMario);
	sprites->Add(ID_SPRITE_MARIO_SMALL_RUNNING_RIGHT + 2, 306, 0, 306 + 15, 15, texMario); 
	sprites->Add(ID_SPRITE_MARIO_SMALL_RUNNING_RIGHT + 3, 335, 0, 335 + 15, 15, texMario);

	sprites->Add(ID_SPRITE_MARIO_SMALL_RUNNING_LEFT + 1, 155, 0, 155 + 15, 15, texMario);
	sprites->Add(ID_SPRITE_MARIO_SMALL_RUNNING_LEFT + 2, 125, 0, 125 + 15, 15, texMario);
	sprites->Add(ID_SPRITE_MARIO_SMALL_RUNNING_LEFT + 3, 95, 0, 95 + 15, 15, texMario);

	sprites->Add(ID_SPRITE_MARIO_SMALL_BRACE_LEFT + 1, 6, 0, 6 + 13, 15, texMario);
	sprites->Add(ID_SPRITE_MARIO_SMALL_BRACE_RIGHT + 1, 426, 0, 426 + 13, 15, texMario);

	sprites->Add(ID_SPRITE_MARIO_SMALL_JUMP_WALK_LEFT + 1, 35, 80, 35 + 15, 80 + 15, texMario);		
	sprites->Add(ID_SPRITE_MARIO_SMALL_JUMP_WALK_RIGHT + 1, 395, 80, 395 + 15, 80 + 15, texMario);

	sprites->Add(ID_SPRITE_MARIO_SMALL_JUMP_RUN_LEFT + 1, 65, 40, 65 + 15, 40 + 15, texMario);
	sprites->Add(ID_SPRITE_MARIO_SMALL_JUMP_RUN_RIGHT + 1, 365, 40, 365 + 15, 40 + 15, texMario);

	LPANIMATION ani;

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_BIG_IDLE_RIGHT + 1);
	animations->Add(ID_ANI_MARIO_IDLE_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_BIG_IDLE_LEFT + 1);
	animations->Add(ID_ANI_MARIO_IDLE_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_BIG_IDLE_RIGHT + 1);
	ani->Add(ID_SPRITE_MARIO_BIG_WALKING_RIGHT + 2);
	ani->Add(ID_SPRITE_MARIO_BIG_WALKING_RIGHT + 3);
	animations->Add(ID_ANI_MARIO_WALKING_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_BIG_IDLE_LEFT + 1);
	ani->Add(ID_SPRITE_MARIO_BIG_WALKING_LEFT + 2);
	ani->Add(ID_SPRITE_MARIO_BIG_WALKING_LEFT + 3);
	animations->Add(ID_ANI_MARIO_WALKING_LEFT, ani);

	ani = new CAnimation(50);
	ani->Add(ID_SPRITE_MARIO_BIG_RUNNING_RIGHT + 1);
	ani->Add(ID_SPRITE_MARIO_BIG_RUNNING_RIGHT + 2);
	ani->Add(ID_SPRITE_MARIO_BIG_RUNNING_RIGHT + 3);
	animations->Add(ID_ANI_MARIO_RUNNING_RIGHT, ani);

	// Mario runs faster hence animation speed should be faster
	ani = new CAnimation(50);	
	ani->Add(ID_SPRITE_MARIO_BIG_RUNNING_LEFT + 1);
	ani->Add(ID_SPRITE_MARIO_BIG_RUNNING_LEFT + 2);
	ani->Add(ID_SPRITE_MARIO_BIG_RUNNING_LEFT + 3);
	animations->Add(ID_ANI_MARIO_RUNNING_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_BIG_JUMP_WALK_RIGHT + 1);
	animations->Add(ID_ANI_MARIO_JUMP_WALK_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_BIG_JUMP_WALK_LEFT + 1);
	animations->Add(ID_ANI_MARIO_JUMP_WALK_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_BIG_JUMP_RUN_RIGHT + 1);
	animations->Add(ID_ANI_MARIO_JUMP_RUN_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_BIG_JUMP_RUN_LEFT + 1);
	animations->Add(ID_ANI_MARIO_JUMP_RUN_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_BIG_SIT_RIGHT + 1);
	animations->Add(ID_ANI_MARIO_SIT_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_BIG_SIT_LEFT + 1);
	animations->Add(ID_ANI_MARIO_SIT_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_BIG_BRACE_RIGHT + 1);
	animations->Add(ID_ANI_MARIO_BRACE_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_BIG_BRACE_LEFT + 1);
	animations->Add(ID_ANI_MARIO_BRACE_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_DIE + 1);
	animations->Add(ID_ANI_MARIO_DIE, ani);

	//
	///////////FOX MARIO
	//
	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_FOX_IDLE_RIGHT + 1);
	animations->Add(ID_ANI_MARIO_FOX_IDLE_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_FOX_IDLE_LEFT + 1);
	animations->Add(ID_ANI_MARIO_FOX_IDLE_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_FOX_IDLE_RIGHT + 1);
	ani->Add(ID_SPRITE_MARIO_FOX_WALKING_RIGHT + 2);
	ani->Add(ID_SPRITE_MARIO_FOX_WALKING_RIGHT + 3);
	animations->Add(ID_ANI_MARIO_FOX_WALKING_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_FOX_IDLE_LEFT + 1);
	ani->Add(ID_SPRITE_MARIO_FOX_WALKING_LEFT + 2);
	ani->Add(ID_SPRITE_MARIO_FOX_WALKING_LEFT + 3);
	animations->Add(ID_ANI_MARIO_FOX_WALKING_LEFT, ani);

	ani = new CAnimation(50);
	ani->Add(ID_SPRITE_MARIO_FOX_RUNNING_RIGHT + 1);
	ani->Add(ID_SPRITE_MARIO_FOX_RUNNING_RIGHT + 2);
	ani->Add(ID_SPRITE_MARIO_FOX_RUNNING_RIGHT + 3);
	animations->Add(ID_ANI_MARIO_FOX_RUNNING_RIGHT, ani);

	// Mario runs faster hence animation speed should be faster
	ani = new CAnimation(50);
	ani->Add(ID_SPRITE_MARIO_FOX_RUNNING_LEFT + 1);
	ani->Add(ID_SPRITE_MARIO_FOX_RUNNING_LEFT + 2);
	ani->Add(ID_SPRITE_MARIO_FOX_RUNNING_LEFT + 3);
	animations->Add(ID_ANI_MARIO_FOX_RUNNING_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_FOX_JUMP_WALK_RIGHT + 1);
	animations->Add(ID_ANI_MARIO_FOX_JUMP_WALK_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_FOX_JUMP_WALK_LEFT + 1);
	animations->Add(ID_ANI_MARIO_FOX_JUMP_WALK_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_FOX_JUMP_RUN_RIGHT + 1);
	animations->Add(ID_ANI_MARIO_FOX_JUMP_RUN_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_FOX_JUMP_RUN_LEFT + 1);
	animations->Add(ID_ANI_MARIO_FOX_JUMP_RUN_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_FOX_SIT_RIGHT + 1);
	animations->Add(ID_ANI_MARIO_FOX_SIT_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_FOX_SIT_LEFT + 1);
	animations->Add(ID_ANI_MARIO_FOX_SIT_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_FOX_BRACE_RIGHT + 1);
	animations->Add(ID_ANI_MARIO_FOX_BRACE_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_FOX_BRACE_LEFT + 1);
	animations->Add(ID_ANI_MARIO_FOX_BRACE_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_FOX_HIT_RIGHT + 1);
	ani->Add(ID_SPRITE_MARIO_FOX_HIT_RIGHT + 2);
	ani->Add(ID_SPRITE_MARIO_FOX_HIT_RIGHT + 3);
	animations->Add(ID_ANI_MARIO_FOX_HIT_RIGHT, ani);


	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_FOX_HIT_LEFT + 1);
	ani->Add(ID_SPRITE_MARIO_FOX_HIT_LEFT + 2);
	ani->Add(ID_SPRITE_MARIO_FOX_HIT_LEFT + 3);
	animations->Add(ID_ANI_MARIO_FOX_HIT_LEFT, ani);

	//
	// SMALL MARIO 
	//
	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_SMALL_IDLE_RIGHT + 1);
	animations->Add(ID_ANI_MARIO_SMALL_IDLE_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_SMALL_IDLE_RIGHT + 1);
	ani->Add(ID_SPRITE_MARIO_SMALL_WALKING_RIGHT + 2);
	ani->Add(ID_SPRITE_MARIO_SMALL_WALKING_RIGHT + 3);
	animations->Add(ID_ANI_MARIO_SMALL_WALKING_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_SMALL_IDLE_LEFT + 1);
	animations->Add(ID_ANI_MARIO_SMALL_IDLE_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_SMALL_IDLE_LEFT + 1);
	ani->Add(ID_SPRITE_MARIO_SMALL_WALKING_LEFT + 2);
	ani->Add(ID_SPRITE_MARIO_SMALL_WALKING_LEFT + 3);
	animations->Add(ID_ANI_MARIO_SMALL_WALKING_LEFT, ani);

	ani = new CAnimation(50);
	ani->Add(ID_SPRITE_MARIO_SMALL_RUNNING_RIGHT + 1);
	ani->Add(ID_SPRITE_MARIO_SMALL_RUNNING_RIGHT + 2);
	ani->Add(ID_SPRITE_MARIO_SMALL_RUNNING_RIGHT + 3);
	animations->Add(ID_ANI_MARIO_SMALL_RUNNING_RIGHT, ani);

	ani = new CAnimation(50);
	ani->Add(ID_SPRITE_MARIO_SMALL_RUNNING_LEFT + 1);
	ani->Add(ID_SPRITE_MARIO_SMALL_RUNNING_LEFT + 2);
	ani->Add(ID_SPRITE_MARIO_SMALL_RUNNING_LEFT + 3);
	animations->Add(ID_ANI_MARIO_SMALL_RUNNING_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_SMALL_BRACE_LEFT + 1);
	animations->Add(ID_ANI_MARIO_SMALL_BRACE_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_SMALL_BRACE_RIGHT + 1);
	animations->Add(ID_ANI_MARIO_SMALL_BRACE_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_SMALL_JUMP_WALK_RIGHT + 1);
	animations->Add(ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_SMALL_JUMP_WALK_LEFT + 1);
	animations->Add(ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_SMALL_JUMP_RUN_LEFT + 1);
	animations->Add(ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_SMALL_JUMP_RUN_RIGHT + 1);
	animations->Add(ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT, ani);
}

void LoadAssetsGoomba()
{
	CTextures* textures = CTextures::GetInstance();
	CSprites* sprites = CSprites::GetInstance();
	CAnimations* animations = CAnimations::GetInstance();

	LPTEXTURE texEnemy = textures->Get(ID_TEX_ENEMY);

	sprites->Add(ID_SPRITE_GOOMBA_WING_FLY + 1, 4 + 61 + 45, 13-7, 22 + 62 + 45, 30 + 3, texEnemy);   //4+61 13-2 22+62 30+2
	sprites->Add(ID_SPRITE_GOOMBA_WING_FLY + 2, 24 + 61 + 49, 13-7, 42 + 64 + 49, 30 + 3, texEnemy);  //24+61 13-2 42+64 30+2

	sprites->Add(ID_SPRITE_GOOMBA_WING_WALK + 1, 4 + 61, 13 - 2, 22 + 62, 30 + 2, texEnemy);  
	sprites->Add(ID_SPRITE_GOOMBA_WING_WALK + 2, 24 + 61, 13 - 2, 42 + 64, 30 + 2, texEnemy);

	sprites->Add(ID_SPRITE_GOOMBA_WALK + 1, 4, 13, 22, 30, texEnemy);   //4 13 22 30
	sprites->Add(ID_SPRITE_GOOMBA_WALK + 2, 24, 13, 42, 30, texEnemy);  //24 13 42 30

	sprites->Add(ID_SPRITE_GOOMBA_DIE + 1, 44, 19, 62, 30, texEnemy);

	LPANIMATION ani = new CAnimation(100);
	ani->Add(ID_SPRITE_GOOMBA_WALK + 1);
	ani->Add(ID_SPRITE_GOOMBA_WALK + 2);
	animations->Add(ID_ANI_GOOMBA_WALKING, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_GOOMBA_WING_WALK + 1);
	ani->Add(ID_SPRITE_GOOMBA_WING_WALK + 2);
	animations->Add(ID_ANI_GOOMBA_WING_WALK, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_GOOMBA_WING_FLY + 1);
	ani->Add(ID_SPRITE_GOOMBA_WING_FLY + 2);
	animations->Add(ID_ANI_GOOMBA_WING_FLY, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_GOOMBA_DIE + 1);
	animations->Add(ID_ANI_GOOMBA_DIE, ani);

}
void LoadAssetsBrick()
{
	CTextures* textures = CTextures::GetInstance();
	CSprites* sprites = CSprites::GetInstance();
	CAnimations* animations = CAnimations::GetInstance();

	LPTEXTURE texMisc = textures->Get(ID_TEX_MISC);
	sprites->Add(ID_SPRITE_BRICK + 1, 372, 153, 372 + 15, 153 + 15, texMisc);

	LPANIMATION ani = new CAnimation(100);
	ani->Add(ID_SPRITE_BRICK + 1);
	animations->Add(ID_ANI_BRICK, ani);
}
void LoadAssetsCoin()
{
	CTextures* textures = CTextures::GetInstance();
	CSprites* sprites = CSprites::GetInstance();
	CAnimations* animations = CAnimations::GetInstance();

	LPTEXTURE texMisc = textures->Get(ID_TEX_MISC);

	sprites->Add(ID_SPRITE_COIN + 1, 303, 99, 303 + 9, 99 + 15, texMisc);
	sprites->Add(ID_SPRITE_COIN + 2, 321, 99, 321 + 9, 99 + 15, texMisc);
	sprites->Add(ID_SPRITE_COIN + 3, 338, 99, 338 + 9, 99 + 15, texMisc);

	LPANIMATION ani = new CAnimation(300);
	ani->Add(ID_SPRITE_COIN + 1);
	ani->Add(ID_SPRITE_COIN + 2);
	ani->Add(ID_SPRITE_COIN + 3);
	animations->Add(ID_ANI_COIN, ani);
}

void LoadAssetsPipe() {
	CTextures* textures = CTextures::GetInstance();
	CSprites* sprites = CSprites::GetInstance();
	CAnimations* animations = CAnimations::GetInstance();

	LPTEXTURE texEnemy = textures->Get(ID_TEX_ENEMY);

	sprites->Add(ID_SPRITE_PIPE + 1, 48, 87, 88, 120, texEnemy);   //48, 87, 88, 120 pipe 48,102,88,125 pipe under

	LPANIMATION ani = new CAnimation(100);
	ani->Add(ID_SPRITE_PIPE + 1);
	ani->Add(ID_SPRITE_PIPE + 2);
	animations->Add(ID_ANY_PIPE, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_PIPE + 1);
	animations->Add(ID_ANY_PIPE, ani);
}

void LoadAssetsUnderPipe() {
	CTextures* textures = CTextures::GetInstance();
	CSprites* sprites = CSprites::GetInstance();
	CAnimations* animations = CAnimations::GetInstance();

	LPTEXTURE texEnemy = textures->Get(ID_TEX_ENEMY);

	sprites->Add(ID_SPRITE_UNDERPIPE + 1, 48, 102, 88, 125, texEnemy);   //48, 87, 88, 120 pipe 48,102,88,125 pipe under

	LPANIMATION ani = new CAnimation(100);
	ani->Add(ID_SPRITE_UNDERPIPE + 1);
	ani->Add(ID_SPRITE_UNDERPIPE + 2);
	animations->Add(ID_ANY_UNDERPIPE, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_UNDERPIPE + 1);
	animations->Add(ID_ANY_UNDERPIPE, ani);
}
void LoadAssetsQuesBox() {
	CTextures* textures = CTextures::GetInstance();
	CSprites* sprites = CSprites::GetInstance();
	CAnimations* animations = CAnimations::GetInstance();

	LPTEXTURE texMisc = textures->Get(ID_TEX_MISC);

	sprites->Add(ID_SPRITE_QUESBOX + 1, 300 , 116, 300 + 15 , 99 + 34, texMisc);
	sprites->Add(ID_SPRITE_QUESBOX + 2, 318, 116, 318 + 15, 99 + 34, texMisc);
	sprites->Add(ID_SPRITE_QUESBOX + 3, 336, 116, 336 + 15, 99 + 34, texMisc);
	sprites->Add(ID_SPRITE_QUESBOX + 4, 354, 116, 354 + 15, 99 + 34, texMisc);
	sprites->Add(ID_SPRITE_QUESBOX + 5, 372, 116, 372 + 15, 99 + 34, texMisc);

	LPANIMATION ani = new CAnimation(400);
	ani->Add(ID_SPRITE_QUESBOX + 1);
	ani->Add(ID_SPRITE_QUESBOX + 2);
	ani->Add(ID_SPRITE_QUESBOX + 3);
	ani->Add(ID_SPRITE_QUESBOX + 4);
	
	animations->Add(ID_ANI_QUESBOX, ani);
	ani = new CAnimation(400);
	ani->Add(ID_SPRITE_QUESBOX + 5);
	animations->Add(ID_ANI_QUESBOX_NOT, ani);
}

void LoadAssetsMushroom() {
	CTextures* textures = CTextures::GetInstance();
	CSprites* sprites = CSprites::GetInstance();
	CAnimations* animations = CAnimations::GetInstance();

	LPTEXTURE texMisc = textures->Get(ID_TEX_MISC);

	sprites->Add(ID_SPRITE_MUSHROOM + 100, 300, 187 + 1, 300 + 15, 170 + 34 + 1, texMisc);

	LPANIMATION ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MUSHROOM + 100);

	animations->Add(ID_ANI_MUSHROOM_WALKING, ani);

}

void LoadAssetsLeaf() {
	CTextures* textures = CTextures::GetInstance();
	CSprites* sprites = CSprites::GetInstance();
	CAnimations* animations = CAnimations::GetInstance();

	LPTEXTURE texMisc = textures->Get(ID_TEX_MISC);

	sprites->Add(ID_SPRITE_LEAF + 100, 300, 187 + 19, 300 + 15, 170 + 34 + 19 , texMisc);

	LPANIMATION ani = new CAnimation(100);
	ani->Add(ID_SPRITE_LEAF + 100);

	animations->Add(ID_ANI_LEAF_FALLING, ani);

}

void LoadAssesCoineffect() {
	CTextures* textures = CTextures::GetInstance();
	CSprites* sprites = CSprites::GetInstance();
	CAnimations* animations = CAnimations::GetInstance();

	LPTEXTURE texMisc = textures->Get(ID_TEX_MISC);

	sprites->Add(ID_SPRITE_COIN + 4, 303, 99, 303 + 9, 99 + 15, texMisc);
	sprites->Add(ID_SPRITE_COIN + 5, 321, 99, 321 + 9, 99 + 15, texMisc);
	sprites->Add(ID_SPRITE_COIN + 6, 338, 99, 338 + 9, 99 + 15, texMisc);

	LPANIMATION ani = new CAnimation(300);
	ani->Add(ID_SPRITE_COIN + 4);
	ani->Add(ID_SPRITE_COIN + 5);
	ani->Add(ID_SPRITE_COIN + 6);
	animations->Add(ID_ANI_COINJUMP, ani);
}

void LoadAssetsOther()
{
	CTextures* textures = CTextures::GetInstance();
	CSprites* sprites = CSprites::GetInstance();


	LPTEXTURE texMisc = textures->Get(ID_TEX_MISC);
	sprites->Add(ID_SPRITE_CLOUD_BEGIN, 390, 117, 390 + 15, 117 + 15, texMisc);
	sprites->Add(ID_SPRITE_CLOUD_MIDDLE, 408, 117, 408 + 15, 117 + 15, texMisc);
	sprites->Add(ID_SPRITE_CLOUD_END, 426, 117, 426 + 15, 117 + 15, texMisc);

}

void LoadAssetskoopa() {
	CTextures* textures = CTextures::GetInstance();
	CSprites* sprites = CSprites::GetInstance();
	CAnimations* animations = CAnimations::GetInstance();

	LPTEXTURE texEnemy = textures->Get(ID_TEX_ENEMY);
	//walikng left red
	sprites->Add(ID_SPRITE_KOOPA_WALKING + 1, 4, 13 + 110, 22 + 5, 30 + 140, texEnemy);   
	sprites->Add(ID_SPRITE_KOOPA_WALKING + 2, 24, 13 + 110, 42 + 5, 30 + 140, texEnemy);  
	
	//walking right red
	sprites->Add(ID_SPRITE_KOOPA_WALKING_RIGHT + 1, 4 + 304, 13 + 110, 22 + 5 + 304, 30 + 140, texEnemy); // 4 + 304, 13 + 110, 22 + 5 + 304, 30 + 140
	sprites->Add(ID_SPRITE_KOOPA_WALKING_RIGHT + 2, 24 + 304, 13 + 110, 42 + 5 + 304, 30 + 140, texEnemy); //24 + 304, 13 + 110, 42 + 5 + 304, 30 + 140

	//walikng left green
	sprites->Add(ID_SPRITE_GREEN_KOOPA_WALKING + 1, 4 + 105, 13 + 110, 22 + 5 + 105, 30 + 140, texEnemy);   
	sprites->Add(ID_SPRITE_GREEN_KOOPA_WALKING + 2, 24 + 108, 13 + 110, 42 + 5 + 108, 30 + 140, texEnemy);  
	
	//walking right green
	sprites->Add(ID_SPRITE_GREEN_KOOPA_WALKING_RIGHT + 1, 4 + 105 + 66, 13 + 110, 22 + 5 + 105 + 66, 30 + 140, texEnemy);  
	sprites->Add(ID_SPRITE_GREEN_KOOPA_WALKING_RIGHT + 2, 24 + 105 + 70+43, 13 + 110, 42 + 5 + 105 + 70+43, 30 + 140, texEnemy); 

	//shell
	sprites->Add(ID_SPRITE_KOOPA_SHELL + 1, 4 + 40, 13 + 110, 22 + 5 + 40, 30 + 140, texEnemy);

	//shell->walking
	sprites->Add(ID_SPRITE_KOOPA_SHELL_CHANGE + 1, 4 + 62, 13 + 110, 22 + 5 + 62, 30 + 140, texEnemy);

	LPANIMATION ani = new CAnimation(100);
	ani->Add(ID_SPRITE_KOOPA_WALKING + 1);
	ani->Add(ID_SPRITE_KOOPA_WALKING + 2);
	animations->Add(ID_ANI_KOOPA_WALKING, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_GREEN_KOOPA_WALKING + 1);
	ani->Add(ID_SPRITE_GREEN_KOOPA_WALKING + 2);
	animations->Add(ID_ANI_GREEN_KOOPA_WALKING, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_KOOPA_WALKING_RIGHT + 1);
	ani->Add(ID_SPRITE_KOOPA_WALKING_RIGHT + 2);
	animations->Add(ID_ANI_KOOPA_WALKING_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_GREEN_KOOPA_WALKING_RIGHT + 1);
	ani->Add(ID_SPRITE_GREEN_KOOPA_WALKING_RIGHT + 2);
	animations->Add(ID_ANI_GREEN_KOOPA_WALKING_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_KOOPA_SHELL + 1);
	animations->Add(ID_ANI_KOOPA_SHELL, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_KOOPA_SHELL_CHANGE + 1);
	animations->Add(ID_ANI_KOOPA_SHELL_CHANGE, ani);
}

void LoadAssetsworldmap() {

	CTextures* textures = CTextures::GetInstance();
	CSprites* sprites = CSprites::GetInstance();
	CAnimations* animations = CAnimations::GetInstance();

	LPTEXTURE texMap = textures->Get(ID_TEX_MAP);
	sprites->Add(ID_SPRITE_WORLDMAP, 15 -2600, 300 -52 -400 , 220 + 2600 , 450 + 400, texMap);


	LPANIMATION ani = new CAnimation(300);
	ani->Add(ID_SPRITE_WORLDMAP);
	animations->Add(ID_ANI_WORLDMAP, ani);


}

void LoadAssetsPlant() {
	CTextures* textures = CTextures::GetInstance();
	CSprites* sprites = CSprites::GetInstance();
	CAnimations* animations = CAnimations::GetInstance();

	LPTEXTURE texEnemy = textures->Get(ID_TEX_ENEMY);

	sprites->Add(ID_SPRITE_PLANT_BITE + 1, 48 + 135 + 1, 87 - 25, 88 + 135 + 1, 120 - 35, texEnemy);
	sprites->Add(ID_SPRITE_PLANT_BITE + 2, 48 + 90 + 1, 87 - 25, 88 + 90 + 1, 120 - 35, texEnemy);

	sprites->Add(ID_SPRITE_PLANT_LEFT, 48, 87 - 25, 88, 120 - 35, texEnemy);

	sprites->Add(ID_SPRITE_PLANT_RIGHT, 48 + 332, 87 - 25, 88 +332 , 120 - 35, texEnemy);


	LPANIMATION ani = new CAnimation(300);
	ani->Add(ID_SPRITE_PLANT_BITE + 1);
	ani->Add(ID_SPRITE_PLANT_BITE + 2);
	animations->Add(ID_ANI_PLANT_BITE, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_PLANT_LEFT);
	animations->Add(ID_ANI_PLANT_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_PLANT_RIGHT);
	animations->Add(ID_ANI_PLANT_RIGHT, ani);

}

/*
	Load all game resources
	In this example: load textures, sprites, animations and mario object

	TO-DO: Improve this function by loading texture,sprite,animation,object from file
*/
void LoadResources()
{
	CTextures* textures = CTextures::GetInstance();

	textures->Add(ID_TEX_MARIO, TEXTURE_PATH_MARIO);
	textures->Add(ID_TEX_ENEMY, TEXTURE_PATH_ENEMY);
	textures->Add(ID_TEX_MISC, TEXTURE_PATH_MISC);
	textures->Add(ID_TEX_BBOX, TEXTURE_PATH_BBOX);
	textures->Add(ID_TEX_MAP, TEXTURE_PATH_MAP);

	LoadAssetsMario();
	LoadAssetsGoomba();
	LoadAssetsBrick();
	LoadAssetsCoin();
	LoadAssetsPipe();
	LoadAssetsUnderPipe();
	LoadAssetsQuesBox();
	LoadAssetsMushroom();
	LoadAssetsLeaf();
	LoadAssetsOther();
	LoadAssesCoineffect();
	LoadAssetsworldmap();
	LoadAssetskoopa();
	LoadAssetsPlant();

}

void ClearScene()
{ 
	list<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		delete (*it);
	}
	objects.clear();
}

#define MARIO_START_X 20.0f
#define MARIO_START_Y 10.0f

#define BRICK_X 1.0f
#define GOOMBA_X 200.0f
#define COIN_X 100.0f
#define PIPE_X 400.0f
#define UNDERPIPE_X 400.0f
#define QUESBOX_X 100.0f
#define MUSHROOM_X 100.0f
#define LEAF_X 100.0f

#define BRICK_Y GROUND_Y + 40.0f
#define NUM_BRICKS 200

/*
* Reload all objects of current scene 
* NOTE: super bad way to build a scene! We need to load a scene from data instead of hard-coding like this 
*/
void Reload()
{
	ClearScene();
	// Main ground

	for (int i = 0; i < 40; i++)
	{
		CBrick* b = new CBrick(i * BRICK_WIDTH * 1.0f, BRICK_Y - 4.0f);
		objects.push_back(b);
	}
	for (int i = 40; i < 68; i++)
	{
		for (int j = 1; j < 3; j++)
		{
			CBrick* b = new CBrick(BRICK_X + i * BRICK_WIDTH - 1.0f, BRICK_Y - j * BRICK_WIDTH + 12.0f);
			objects.push_back(b);
	
		}
	}

	for (int i = 73; i < 94; i++)
	{
		CBrick* b = new CBrick(i * BRICK_WIDTH * 1.0f, BRICK_Y - 4.0f);
		objects.push_back(b);
	}
	for (int i = 97; i < 101; i++)
	{
		CBrick* b = new CBrick(i * BRICK_WIDTH * 1.0f, BRICK_Y - 4.0f);
		objects.push_back(b);
	}
	for (int i = 105; i < 140; i++)
	{
		CBrick* b = new CBrick(i * BRICK_WIDTH * 1.0f, BRICK_Y - 4.0f);
		objects.push_back(b);
	}
	for (int i = 142; i < 200; i++)
	{
		CBrick* b = new CBrick(i * BRICK_WIDTH * 1.0f, BRICK_Y - 4.0f);
		objects.push_back(b);
	}
	for (int i = 0; i < 2; i++)
	{
		CBrick* b = new CBrick(1511.0f + BRICK_WIDTH * i, GROUND_Y - 44.0f);
		objects.push_back(b);
	}
	for (int i = 0; i < 3; i++)
	{
		CBrick* b = new CBrick(1576.0f + BRICK_WIDTH * i, GROUND_Y + 20.0f - i * 16.0f );
		objects.push_back(b);
	}
	for (int i = 0; i < 3; i++)
	{
		CBrick* b = new CBrick(1671.0f + BRICK_WIDTH * i, GROUND_Y - 12.0f + i* 16.0f );
		objects.push_back(b);
	}

	// Vertical column 1
	for (int i = 0; i < 10; i++)
	{
		CBrick* b = new CBrick(-8.0f, BRICK_Y - i * BRICK_WIDTH);
		objects.push_back(b);
	}



	// Second cloud platform
	for (int i = 0; i < 2; i++) {
		CPlatform* p = new CPlatform(248.0f + i * 32, GROUND_Y - 13.0f - 32.0f * i,
			15, 15, 3, ID_SPRITE_CLOUD_MIDDLE, ID_SPRITE_CLOUD_MIDDLE, ID_SPRITE_CLOUD_MIDDLE);
		objects.push_back(p);
	}
	for (int i = 0; i < 2; i++) {
		CPlatform* p = new CPlatform(472.0f + i * 48, GROUND_Y  - 45.0f - 32.0f * i,
			15, 15, 4, ID_SPRITE_CLOUD_MIDDLE, ID_SPRITE_CLOUD_MIDDLE, ID_SPRITE_CLOUD_MIDDLE);
		objects.push_back(p);
	}
	for (int i = 1; i < 2; i++) {
		CPlatform* p = new CPlatform(407.0f + i , GROUND_Y  - 13.0f ,
			15, 15, 5, ID_SPRITE_CLOUD_MIDDLE, ID_SPRITE_CLOUD_MIDDLE, ID_SPRITE_CLOUD_MIDDLE);
		objects.push_back(p);
	}
	for (int i = 1; i < 2; i++) {
		CPlatform* p = new CPlatform(520.0f + i, GROUND_Y + 3.0f  ,
			15, 15, 6, ID_SPRITE_CLOUD_MIDDLE, ID_SPRITE_CLOUD_MIDDLE, ID_SPRITE_CLOUD_MIDDLE);
		objects.push_back(p);
	}
	for (int i = 0; i < 3; i++) {
		CPlatform* p = new CPlatform(1272.0f + i * 32, GROUND_Y + 4.0f - 32.0f * i,
			16, 15, 7, ID_SPRITE_CLOUD_MIDDLE, ID_SPRITE_CLOUD_MIDDLE, ID_SPRITE_CLOUD_MIDDLE);
		objects.push_back(p);
	}
	for (int i = 0; i < 2; i++) {
		CPlatform* p = new CPlatform(2152.0f + i * 32.0f , GROUND_Y - 13.0f - 96.0f * i,
			15, 15, 3, ID_SPRITE_CLOUD_MIDDLE, ID_SPRITE_CLOUD_MIDDLE, ID_SPRITE_CLOUD_MIDDLE);
		objects.push_back(p);
	}
	for (int i = 2; i < 3; i++) {
		CPlatform* p = new CPlatform(1256.0f + i * 32, GROUND_Y - 120.0f * i + 3.0f,
			15, 15, 4, ID_SPRITE_CLOUD_MIDDLE, ID_SPRITE_CLOUD_MIDDLE, ID_SPRITE_CLOUD_MIDDLE);
		objects.push_back(p);
	}
	for (int i = 2; i < 3; i++) {
		CPlatform* p = new CPlatform(1352.0f + i * 32, GROUND_Y - 94.0f * i - 1.0f,
			15, 15, 4, ID_SPRITE_CLOUD_MIDDLE, ID_SPRITE_CLOUD_MIDDLE, ID_SPRITE_CLOUD_MIDDLE);
		objects.push_back(p);
	}
	for (int i = 2; i < 3; i++) {
		CPlatform* p = new CPlatform(1432.0f + i * 32, GROUND_Y - 100.0f * i - 5.0f,
			15, 15, 14, ID_SPRITE_CLOUD_MIDDLE, ID_SPRITE_CLOUD_MIDDLE, ID_SPRITE_CLOUD_MIDDLE);
		objects.push_back(p);
	}



	//pipe
	for (int j = 1; j < 2; j++)
	{
		Cunderpipe* underpipe = new Cunderpipe(UNDERPIPE_X * 4 + 272.0f + j * 400.0f, GROUND_Y + 24.0f);
		objects.push_back(underpipe);
	}
	for (int j = 1; j < 2; j++)
	{
		Cpipe* pipe = new Cpipe(PIPE_X * 4 + 272.0f + j * 400.0f, GROUND_Y - 2.0f);
		objects.push_back(pipe);
	}
	for (int j = 0; j < 11; j++)
	{
		Cunderpipe* underpipe = new Cunderpipe(UNDERPIPE_X * 4 + 295.0f + 376.0f, GROUND_Y - 85.0f - 16.0f * j);
		objects.push_back(underpipe);
	}
	for (int j = 1; j < 2; j++)
	{
		Cpipe* pipe = new Cpipe(PIPE_X * 4 + 295.0f + j * 376.0f, GROUND_Y - 262.0f);
		objects.push_back(pipe);
	}
	//ogrange brick
	for (int i = 1; i < 3; i++)
	{
		CBrick* b = new CBrick(i * BRICK_WIDTH + 2247.0f, BRICK_Y - 52.0f);
		objects.push_back(b);
	}
	for (int i = 1; i < 3; i++)
	{
		CBrick* b = new CBrick(i * BRICK_WIDTH + 2247.0f, BRICK_Y - 117.0f);
		objects.push_back(b);
	}

	//world map

	for (int i = 1; i < 2; i++)
	{
		Cworldmap* w = new Cworldmap(i + 100.0f, GROUND_Y - 55.0f);
		objects.push_back(w);
	}

	//enemies
	for (int j = 0; j < 1; j++)
	{
		CPlant* plant = new CPlant(366.0f, GROUND_Y - 12.0f);
		objects.push_back(plant);
	}
	for (int j = 0; j < 1; j++)
	{
		CGoomba* goomba = new CGoomba(GOOMBA_X + 300.0f, GROUND_Y - 20.0f, 0);
		objects.push_back(goomba);
	}
	//for (int j = 0; j < 1; j++)
	//{
	//	CKoopa* koopa = new CKoopa(150.0f, GROUND_Y - 48.0f, 1);
	//	objects.push_back(koopa);
	//}
	for (int j = 0; j < 1; j++)
	{
		CKoopa* koopa = new CKoopa(550.0f, GROUND_Y - 15.0f, 0);
		objects.push_back(koopa);
	}

	//pipe outside
	for (int j = 0; j < 1; j++)
	{
		Cunderpipe* underpipe = new Cunderpipe(UNDERPIPE_X + j - 32.0f, GROUND_Y + 24.0f);
		objects.push_back(underpipe);
	}
	for (int j = 0; j < 1; j++)
	{
		Cpipe* pipe = new Cpipe(PIPE_X + j - 32.0f, GROUND_Y - 2.0f);
		objects.push_back(pipe);
	}
	for (int j = 0; j < 2; j++)
	{
		Cpipe* pipe = new Cpipe(PIPE_X * 4 + 208.0f + j * 527.0f, GROUND_Y + 13.0f);
		objects.push_back(pipe);
	}
	for (int j = 0; j < 1; j++)
	{
		Cunderpipe* underpipe = new Cunderpipe(UNDERPIPE_X * 4 + 272.0f + j * 400.0f, GROUND_Y + 24.0f);
		objects.push_back(underpipe);
	}
	for (int j = 0; j < 1; j++)
	{
		Cpipe* pipe = new Cpipe(PIPE_X * 4 + 272.0f + j * 400.0f, GROUND_Y - 2.0f);
		objects.push_back(pipe);
	}

	//quesbox

	for (int j = 1; j < 2; j++)
	{

		CMushroom* mushroom = new CMushroom(130.0f + MUSHROOM_X + j * QUESBOX_WIDTH, GROUND_Y - 76.0f);
		objects.push_back(mushroom);
	}
	for (int i = 0; i < 1; i++)
	{
		CMushroom* m = new CMushroom(1380.0f + QUESBOX_X + i, GROUND_Y - 13.0f);
		objects.push_back(m);
	}
	for (int i = 0; i < 1; i++)
	{
		CMushroom* mushroom = new CMushroom(564.0f + QUESBOX_X + i * (QUESBOX_WIDTH) * 3.0f, GROUND_Y + 5.0f);
		objects.push_back(mushroom);
	}
	for (int i = 0; i < 1; i++)
	{
		CLeaf* leaf = new CLeaf(564.0f + QUESBOX_X + i * (QUESBOX_WIDTH) * 3.0f, GROUND_Y + 5.0f);
		objects.push_back(leaf);
	}
	for (int j = 1; j < 2; j++)
	{

		CLeaf* leaf = new CLeaf(130.0f + MUSHROOM_X + j * QUESBOX_WIDTH, GROUND_Y - 76.0f);
		objects.push_back(leaf);
	}
	for (int i = 0; i < 1; i++)
	{
		CLeaf* l = new CLeaf(1380.0f + QUESBOX_X + i, GROUND_Y - 13.0f);
		objects.push_back(l);
	}
	for (int i = 0; i < 2; i++)
	{
		CCoinjump* c = new CCoinjump(80.0f + QUESBOX_X + i * (QUESBOX_WIDTH)+2.0f, GROUND_Y - 28.0f + 0.5f);
		objects.push_back(c);
	}
	for (int i = 0; i < 1; i++)
	{
		CCoinjump* c = new CCoinjump(130.0f + QUESBOX_X + i * (QUESBOX_WIDTH), GROUND_Y - 76.0f );
		objects.push_back(c);
	}
	for (int i = 1; i < 2; i++)
	{
		CCoinjump* c = new CCoinjump(307.0f + QUESBOX_X + i * (QUESBOX_WIDTH), GROUND_Y - 61.0f);
		objects.push_back(c);
	}
	for (int i = 1; i < 2; i++)
	{
		CCoinjump* c = new CCoinjump(564.0f + QUESBOX_X + i * (QUESBOX_WIDTH) * 3.0f, GROUND_Y + 4.0f - 32.0f * i);
		objects.push_back(c);
	}
	for (int i = 0; i < 2; i++)
	{
		CQuesbox* c = new CQuesbox(80.0f + QUESBOX_X + i * (QUESBOX_WIDTH) + 2.0f , GROUND_Y - 28.0f);
		objects.push_back(c);
	}
	for (int i = 0; i < 2; i++)
	{
		CQuesbox* c = new CQuesbox(130.0f + QUESBOX_X + i * (QUESBOX_WIDTH), GROUND_Y - 76.0f);
		objects.push_back(c);
	}
	for (int i = 1; i < 2; i++)
	{
		CQuesbox* c = new CQuesbox(307.0f + QUESBOX_X + i * (QUESBOX_WIDTH), GROUND_Y - 61.0f);
		objects.push_back(c);
	}
	for (int i = 0; i < 2; i++)
	{
		CQuesbox* c = new CQuesbox(564.0f + QUESBOX_X + i * (QUESBOX_WIDTH) * 3.0f , GROUND_Y + 4.0f - 32.0f * i );
		objects.push_back(c);
	}
	for (int i = 0; i < 1; i++)
	{
		CQuesbox* c = new CQuesbox(1380.0f + QUESBOX_X + i, GROUND_Y - 13.0f );
		objects.push_back(c);
	}
	for (int i = 0; i < 1; i++)
	{
		CQuesbox* b = new CQuesbox(1447.0f + i * 16.0f, GROUND_Y - 252.0f);
		objects.push_back(b);
	}
	// COINS 
	for (int i = 0; i < 5 ; i++)
	{
		CCoin* c = new CCoin(1080.0f + i * 30.0f , GROUND_Y - 60.0f - 32.0f * i);
		objects.push_back(c);
	}
	for (int i = 0; i < 3; i++)
	{
		CCoin* c = new CCoin(1333.0f + i * 16.0f, GROUND_Y - 300.0f);
		objects.push_back(c);
	}
	for (int i = 0; i < 2; i++)
	{
		CCoin* c = new CCoin(1477.0f + i * 16.0f, GROUND_Y - 270.0f);
		objects.push_back(c);
	}
	for (int i = 0; i < 4; i++)
	{
		CCoin* c = new CCoin(1542.0f + i * 16.0f, GROUND_Y - 252.0f);
		objects.push_back(c);
	}
	for (int i = 0; i < 4; i++)
	{
		CCoin* c = new CCoin(1620.0f + i * 16.0f, GROUND_Y - 270.0f);
		objects.push_back(c);
	}


	//mario
	mario = new CMario(MARIO_START_X, MARIO_START_Y);
	objects.push_back(mario);
}

bool IsGameObjectDeleted(const LPGAMEOBJECT& o) { return o == NULL; }

void PurgeDeletedObjects()
{
	list<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		LPGAMEOBJECT o = *it;
		if (o->IsDeleted())
		{
			delete o;
			*it = NULL;
		}
	}
	objects.erase(
		std::remove_if(objects.begin(), objects.end(), IsGameObjectDeleted),
		objects.end());
}

/*
	Update world status for this frame
	dt: time period between beginning of last frame and beginning of this frame
*/
void Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 
	vector<LPGAMEOBJECT> coObjects;
	list<LPGAMEOBJECT>::iterator i;
	for (i = objects.begin(); i != objects.end(); ++i)
	{
		coObjects.push_back(*i);
	}

	for (i = objects.begin(); i != objects.end(); ++i)
	{
		(*i)->Update(dt,&coObjects);
	}

	PurgeDeletedObjects();

	// Update camera to follow mario
	float cx, cy;
	mario->GetPosition(cx, cy);

	cx -= SCREEN_WIDTH / 2;
	//cy = 0;
	cy -= SCREEN_HEIGHT / 8;

	if (cx < 0) cx = 0;
	if (cy > 0) cy = 0;

	CGame::GetInstance()->SetCamPos(cx, cy);
}

/*
	Render a frame 
*/
void Render()
{
	CGame* g = CGame::GetInstance();

	ID3D10Device* pD3DDevice = g->GetDirect3DDevice();
	IDXGISwapChain* pSwapChain = g->GetSwapChain();
	ID3D10RenderTargetView* pRenderTargetView = g->GetRenderTargetView();
	ID3DX10Sprite* spriteHandler = g->GetSpriteHandler();

	pD3DDevice->ClearRenderTargetView(pRenderTargetView, BACKGROUND_COLOR);

	spriteHandler->Begin(D3DX10_SPRITE_SORT_TEXTURE);

	FLOAT NewBlendFactor[4] = { 0,0,0,0 };
	pD3DDevice->OMSetBlendState(g->GetAlphaBlending(), NewBlendFactor, 0xffffffff);

	list<LPGAMEOBJECT>::iterator i;
	for (i = objects.begin(); i != objects.end(); ++i)
	{
		(*i)->Render();
	}

	spriteHandler->End();
	pSwapChain->Present(0, 0);
}

HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;

	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = (HICON)LoadImage(hInstance, WINDOW_ICON_PATH, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	HWND hWnd =
		CreateWindow(
			WINDOW_CLASS_NAME,
			MAIN_WINDOW_TITLE,
			WS_OVERLAPPEDWINDOW, // WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			ScreenWidth,
			ScreenHeight,
			NULL,
			NULL,
			hInstance,
			NULL);

	if (!hWnd) 
	{
		OutputDebugString(L"[ERROR] CreateWindow failed");
		DWORD ErrCode = GetLastError();
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}

int Run()
{
	MSG msg;
	int done = 0;
	ULONGLONG frameStart = GetTickCount64();
	DWORD tickPerFrame = 1000 / MAX_FRAME_RATE;

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) done = 1;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		ULONGLONG now = GetTickCount64();

		// dt: the time between (beginning of last frame) and now
		// this frame: the frame we are about to render
		DWORD dt = (DWORD)(now - frameStart);

		if (dt >= tickPerFrame)
		{
			frameStart = now;

			game->ProcessKeyboard();			
			Update(dt);
			Render();
		}
		else
			Sleep(tickPerFrame - dt);	
	}

	return 1;
}

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow
) {
	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);

	SetDebugWindow(hWnd);

	game = CGame::GetInstance();
	game->Init(hWnd, hInstance);

	keyHandler = new CSampleKeyHandler();
	game->InitKeyboard(keyHandler);

	LoadResources();
	Reload();

	SetWindowPos(hWnd, 0, 0, 0, SCREEN_WIDTH*2, SCREEN_HEIGHT*2, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);

	Run();

	return 0;
}
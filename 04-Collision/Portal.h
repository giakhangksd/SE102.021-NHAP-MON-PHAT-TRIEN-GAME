#pragma once
#pragma once

#include "AssetIDs.h"
#include "GameObject.h"

#define GIFT_FLYING_SPEED					0.02f

#define GIFT_TYPE_MUSHROOM					1
#define GIFT_TYPE_FLOWER					2
#define GIFT_TYPE_STAR						3

#define ID_ANI_GIFT_MUSHROOM				11100
#define ID_ANI_GIFT_FLOWER					11101
#define ID_ANI_GIFT_STAR					11102

#define PORTAL_TYPE_PLAYSCENE_TO_ANOTHER	1
#define PORTAL_TYPE_ANOTHER_TO_PLAYSCENE	2

#define GOAL_WIDTH							247
#define GOAL_HEIGHT							73

#define SWITCHING_GIFT_TIME					100
#define SWITCHING_SCENE_TIME				2000

/*
	Object that triggers scene switching
*/
class CPortal : public CGameObject
{
	int type;
	int scene_id;	// target scene to switch to 
	int giftType;

	float width;
	float height;

	float goal_x;
	float goal_y;

	ULONGLONG switchGift_start;
	ULONGLONG switchScene_start;

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

public:
	CPortal(float l, float t, float r, float b, int scene_id, int Type = 1);

	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);

	void RenderBoundingBox(void);

	int GetSceneId() {
		return scene_id;
	}

	int IsBlocking() {
		return 0;
	}

	void SwitchScene();
	int IsSwitchingScene() {
		return (int)switchScene_start;
	}

	int GetSpriteGift() {
		switch (giftType) {
		case GIFT_TYPE_MUSHROOM:
			return ID_SPRITE_GIFT_MUSHROOM_ON_HUD;
		case GIFT_TYPE_FLOWER:
			return ID_SPRITE_GIFT_FLOWER_ON_HUD;
		default:
			return ID_SPRITE_GIFT_STAR_ON_HUD;
		}
	}
};

#pragma once

#include "Hud.h"
#include "Tail.h"
#include "debug.h"
#include "Animation.h"
#include "Animations.h"
#include "GameObject.h"
#include "KoopaTroopa.h"


#define MARIO_WALKING_SPEED							0.1f
#define MARIO_RUNNING_SPEED							0.2f
#define MARIO_ATTACK_SPEED							1.0f
#define MARIO_SPEED_UP_SPEED						0.15f

#define MARIO_ACCEL_WALK_X							0.0005f
#define MARIO_ACCEL_RUN_X							0.0001f
#define MARIO_ACCEL_CHANGE_DIRECTION				0.0007f

#define MARIO_JUMP_SPEED_Y							0.38f
#define MARIO_JUMP_RUN_SPEED_Y						0.45f

#define MARIO_FLY_SPEED_Y							0.2f
#define MARIO_FLOAT_SPEED_Y							0.06f

#define MARIO_GRAVITY								0.001f

#define MARIO_JUMP_DEFLECT_SPEED					0.2f

#define MARIO_STATE_DIE								-10
#define MARIO_STATE_IDLE							0

#define MARIO_STATE_WALKING_RIGHT					100
#define MARIO_STATE_WALKING_LEFT					200

#define MARIO_STATE_JUMP							300
#define MARIO_STATE_RELEASE_JUMP					301

#define MARIO_STATE_RUNNING_RIGHT					400
#define MARIO_STATE_RUNNING_LEFT					500

#define MARIO_STATE_SIT								600
#define MARIO_STATE_SIT_RELEASE						601

#define MARIO_STATE_ATTACK							700
#define MARIO_STATE_KICK							701

#define MARIO_STATE_HOLD							800
#define MARIO_STATE_HOLD_RELEASE					801

#define MARIO_STATE_FLY								900
#define MARIO_STATE_RELEASE_FLY						901


#pragma region ANIMATION_ID_MARIO

// SMALL MARIO
#define ID_ANI_MARIO_SMALL_IDLE_LEFT				100
#define ID_ANI_MARIO_SMALL_IDLE_RIGHT				101
#define ID_ANI_MARIO_SMALL_GET_INTO_PIPE			102

#define ID_ANI_MARIO_SMALL_WALKING_LEFT				200
#define ID_ANI_MARIO_SMALL_WALKING_RIGHT			201

#define ID_ANI_MARIO_SMALL_RUNNING_LEFT				300
#define ID_ANI_MARIO_SMALL_RUNNING_RIGHT			301

#define ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT			400
#define ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT			401

#define ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT			500
#define ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT			501

#define ID_ANI_MARIO_SMALL_BRACE_LEFT				600
#define ID_ANI_MARIO_SMALL_BRACE_RIGHT				601

#define ID_ANI_MARIO_SMALL_TRANSFORM_BIG_LEFT		610
#define ID_ANI_MARIO_SMALL_TRANSFORM_BIG_RIGHT		611

// BIG MARIO
#define ID_ANI_MARIO_IDLE_LEFT						700
#define ID_ANI_MARIO_IDLE_RIGHT						701
#define ID_ANI_MARIO_GET_INTO_PIPE					702

#define ID_ANI_MARIO_WALKING_LEFT					800
#define ID_ANI_MARIO_WALKING_RIGHT					801

#define ID_ANI_MARIO_RUNNING_LEFT					900
#define ID_ANI_MARIO_RUNNING_RIGHT					901

#define ID_ANI_MARIO_JUMP_WALK_LEFT					1000
#define ID_ANI_MARIO_JUMP_WALK_RIGHT				1001

#define ID_ANI_MARIO_JUMP_RUN_LEFT					1100
#define ID_ANI_MARIO_JUMP_RUN_RIGHT					1101

#define ID_ANI_MARIO_SIT_LEFT						1200
#define ID_ANI_MARIO_SIT_RIGHT						1201

#define ID_ANI_MARIO_BRACE_LEFT						1300
#define ID_ANI_MARIO_BRACE_RIGHT					1301

// RACOON MARIO
#define ID_ANI_MARIO_RACOON_IDLE_LEFT				1400
#define ID_ANI_MARIO_RACOON_IDLE_RIGHT				1401
#define ID_ANI_MARIO_RACOON_IDLE_FRONT				1402

#define ID_ANI_MARIO_RACOON_WALKING_LEFT			1500
#define ID_ANI_MARIO_RACOON_WALKING_RIGHT			1501

#define ID_ANI_MARIO_RACOON_RUNNING_LEFT			1600
#define ID_ANI_MARIO_RACOON_RUNNING_RIGHT			1601

#define ID_ANI_MARIO_RACOON_JUMP_WALK_UP_LEFT		1700
#define ID_ANI_MARIO_RACOON_JUMP_WALK_UP_RIGHT		1701

#define ID_ANI_MARIO_RACOON_JUMP_WALK_DOWN_LEFT		1702
#define ID_ANI_MARIO_RACOON_JUMP_WALK_DOWN_RIGHT	1703

#define ID_ANI_MARIO_RACOON_JUMP_RUN_UP_LEFT		1800
#define ID_ANI_MARIO_RACOON_JUMP_RUN_UP_RIGHT		1801

#define ID_ANI_MARIO_RACOON_JUMP_RUN_DOWN_LEFT		1802
#define ID_ANI_MARIO_RACOON_JUMP_RUN_DOWN_RIGHT		1803

#define ID_ANI_MARIO_RACOON_SIT_LEFT				1900
#define ID_ANI_MARIO_RACOON_SIT_RIGHT				1901

#define ID_ANI_MARIO_RACOON_BRACE_LEFT				2000
#define ID_ANI_MARIO_RACOON_BRACE_RIGHT				2001

#define ID_ANI_MARIO_RACOON_ATTACK_LEFT				2100
#define ID_ANI_MARIO_RACOON_ATTACK_RIGHT			2101

// KICK
#define ID_ANI_MARIO_SMALL_KICK_LEFT				2200
#define ID_ANI_MARIO_SMALL_KICK_RIGHT				2201

#define ID_ANI_MARIO_KICK_LEFT						2300
#define ID_ANI_MARIO_KICK_RIGHT						2301

#define ID_ANI_MARIO_RACOON_KICK_LEFT				2400
#define ID_ANI_MARIO_RACOON_KICK_RIGHT				2401

// HOLD IDLE
#define ID_ANI_MARIO_SMALL_HOLD_IDLE_LEFT			2500
#define ID_ANI_MARIO_SMALL_HOLD_IDLE_RIGHT			2501

#define ID_ANI_MARIO_HOLD_IDLE_LEFT					2600
#define ID_ANI_MARIO_HOLD_IDLE_RIGHT				2601

#define ID_ANI_MARIO_RACOON_HOLD_IDLE_LEFT			2700
#define ID_ANI_MARIO_RACOON_HOLD_IDLE_RIGHT			2701

// HOLD RUN
#define ID_ANI_MARIO_SMALL_HOLD_RUN_LEFT			2800
#define ID_ANI_MARIO_SMALL_HOLD_RUN_RIGHT			2801

#define ID_ANI_MARIO_HOLD_RUN_LEFT					2900
#define ID_ANI_MARIO_HOLD_RUN_RIGHT					2901

#define ID_ANI_MARIO_RACOON_HOLD_RUN_LEFT			3000
#define ID_ANI_MARIO_RACOON_HOLD_RUN_RIGHT			3001

// HOLD JUMP
#define ID_ANI_MARIO_SMALL_HOLD_JUMP_LEFT			3100
#define ID_ANI_MARIO_SMALL_HOLD_JUMP_RIGHT			3101

#define ID_ANI_MARIO_HOLD_JUMP_LEFT					3200
#define ID_ANI_MARIO_HOLD_JUMP_RIGHT				3201

#define ID_ANI_MARIO_RACOON_HOLD_JUMP_LEFT			3300
#define ID_ANI_MARIO_RACOON_HOLD_JUMP_RIGHT			3301

// RACOON FLY
#define ID_ANI_MARIO_RACOON_FLY_LEFT				3400
#define ID_ANI_MARIO_RACOON_FLY_RIGHT				3401

#define ID_ANI_MARIO_RACOON_FLOAT_LEFT				3410
#define ID_ANI_MARIO_RACOON_FLOAT_RIGHT				3411

// SPEED UP
#define ID_ANI_MARIO_SMALL_SPEED_UP_LEFT			210
#define ID_ANI_MARIO_SMALL_SPEED_UP_RIGHT			211

#define ID_ANI_MARIO_SPEED_UP_LEFT					810
#define ID_ANI_MARIO_SPEED_UP_RIGHT					811

#define ID_ANI_MARIO_RACOON_SPEED_UP_LEFT			1520
#define ID_ANI_MARIO_RACOON_SPEED_UP_RIGHT			1521

// DIE
#define ID_ANI_MARIO_DIE							999

#pragma endregion

#pragma region ANIMATION_ID_LUIGI

// SMALL MARIO
#define ID_ANI_LUIGI_SMALL_IDLE_LEFT				150
#define ID_ANI_LUIGI_SMALL_IDLE_RIGHT				151
#define ID_ANI_LUIGI_SMALL_GET_INTO_PIPE			152

#define ID_ANI_LUIGI_SMALL_WALKING_LEFT				250
#define ID_ANI_LUIGI_SMALL_WALKING_RIGHT			251

#define ID_ANI_LUIGI_SMALL_RUNNING_LEFT				350
#define ID_ANI_LUIGI_SMALL_RUNNING_RIGHT			351

#define ID_ANI_LUIGI_SMALL_JUMP_WALK_LEFT			450
#define ID_ANI_LUIGI_SMALL_JUMP_WALK_RIGHT			451

#define ID_ANI_LUIGI_SMALL_JUMP_RUN_LEFT			550
#define ID_ANI_LUIGI_SMALL_JUMP_RUN_RIGHT			551

#define ID_ANI_LUIGI_SMALL_BRACE_LEFT				650
#define ID_ANI_LUIGI_SMALL_BRACE_RIGHT				651

#define ID_ANI_LUIGI_SMALL_TRANSFORM_BIG_LEFT		660
#define ID_ANI_LUIGI_SMALL_TRANSFORM_BIG_RIGHT		661

// BIG MARIO
#define ID_ANI_LUIGI_IDLE_LEFT						750
#define ID_ANI_LUIGI_IDLE_RIGHT						751
#define ID_ANI_LUIGI_GET_INTO_PIPE					752

#define ID_ANI_LUIGI_WALKING_LEFT					850
#define ID_ANI_LUIGI_WALKING_RIGHT					851

#define ID_ANI_LUIGI_RUNNING_LEFT					950
#define ID_ANI_LUIGI_RUNNING_RIGHT					951

#define ID_ANI_LUIGI_JUMP_WALK_LEFT					1050
#define ID_ANI_LUIGI_JUMP_WALK_RIGHT				1051

#define ID_ANI_LUIGI_JUMP_RUN_LEFT					1150
#define ID_ANI_LUIGI_JUMP_RUN_RIGHT					1151

#define ID_ANI_LUIGI_SIT_LEFT						1250
#define ID_ANI_LUIGI_SIT_RIGHT						1251

#define ID_ANI_LUIGI_BRACE_LEFT						1350
#define ID_ANI_LUIGI_BRACE_RIGHT					1351

// RACOON MARIO
#define ID_ANI_LUIGI_RACOON_IDLE_LEFT				1450
#define ID_ANI_LUIGI_RACOON_IDLE_RIGHT				1451
#define ID_ANI_LUIGI_RACOON_IDLE_FRONT				1452

#define ID_ANI_LUIGI_RACOON_WALKING_LEFT			1550
#define ID_ANI_LUIGI_RACOON_WALKING_RIGHT			1551

#define ID_ANI_LUIGI_RACOON_RUNNING_LEFT			1650
#define ID_ANI_LUIGI_RACOON_RUNNING_RIGHT			1651

#define ID_ANI_LUIGI_RACOON_JUMP_WALK_UP_LEFT		1750
#define ID_ANI_LUIGI_RACOON_JUMP_WALK_UP_RIGHT		1751

#define ID_ANI_LUIGI_RACOON_JUMP_WALK_DOWN_LEFT		1752
#define ID_ANI_LUIGI_RACOON_JUMP_WALK_DOWN_RIGHT	1753

#define ID_ANI_LUIGI_RACOON_JUMP_RUN_UP_LEFT		1850
#define ID_ANI_LUIGI_RACOON_JUMP_RUN_UP_RIGHT		1851

#define ID_ANI_LUIGI_RACOON_JUMP_RUN_DOWN_LEFT		1852
#define ID_ANI_LUIGI_RACOON_JUMP_RUN_DOWN_RIGHT		1853

#define ID_ANI_LUIGI_RACOON_SIT_LEFT				1950
#define ID_ANI_LUIGI_RACOON_SIT_RIGHT				1951

#define ID_ANI_LUIGI_RACOON_BRACE_LEFT				2050
#define ID_ANI_LUIGI_RACOON_BRACE_RIGHT				2051

#define ID_ANI_LUIGI_RACOON_ATTACK_LEFT				2150
#define ID_ANI_LUIGI_RACOON_ATTACK_RIGHT			2151

// KICK
#define ID_ANI_LUIGI_SMALL_KICK_LEFT				2250
#define ID_ANI_LUIGI_SMALL_KICK_RIGHT				2251

#define ID_ANI_LUIGI_KICK_LEFT						2350
#define ID_ANI_LUIGI_KICK_RIGHT						2351

#define ID_ANI_LUIGI_RACOON_KICK_LEFT				2450
#define ID_ANI_LUIGI_RACOON_KICK_RIGHT				2451

// HOLD IDLE
#define ID_ANI_LUIGI_SMALL_HOLD_IDLE_LEFT			2550
#define ID_ANI_LUIGI_SMALL_HOLD_IDLE_RIGHT			2551

#define ID_ANI_LUIGI_HOLD_IDLE_LEFT					2650
#define ID_ANI_LUIGI_HOLD_IDLE_RIGHT				2651

#define ID_ANI_LUIGI_RACOON_HOLD_IDLE_LEFT			2750
#define ID_ANI_LUIGI_RACOON_HOLD_IDLE_RIGHT			2751

// HOLD RUN
#define ID_ANI_LUIGI_SMALL_HOLD_RUN_LEFT			2850
#define ID_ANI_LUIGI_SMALL_HOLD_RUN_RIGHT			2851

#define ID_ANI_LUIGI_HOLD_RUN_LEFT					2950
#define ID_ANI_LUIGI_HOLD_RUN_RIGHT					2951

#define ID_ANI_LUIGI_RACOON_HOLD_RUN_LEFT			3050
#define ID_ANI_LUIGI_RACOON_HOLD_RUN_RIGHT			3051

// HOLD JUMP
#define ID_ANI_LUIGI_SMALL_HOLD_JUMP_LEFT			3150
#define ID_ANI_LUIGI_SMALL_HOLD_JUMP_RIGHT			3151

#define ID_ANI_LUIGI_HOLD_JUMP_LEFT					3250
#define ID_ANI_LUIGI_HOLD_JUMP_RIGHT				3251

#define ID_ANI_LUIGI_RACOON_HOLD_JUMP_LEFT			3350
#define ID_ANI_LUIGI_RACOON_HOLD_JUMP_RIGHT			3351

// RACOON FLY
#define ID_ANI_LUIGI_RACOON_FLY_LEFT				3450
#define ID_ANI_LUIGI_RACOON_FLY_RIGHT				3451

#define ID_ANI_LUIGI_RACOON_FLOAT_LEFT				3460
#define ID_ANI_LUIGI_RACOON_FLOAT_RIGHT				3461

// SPEED UP
#define ID_ANI_LUIGI_SMALL_SPEED_UP_LEFT			260
#define ID_ANI_LUIGI_SMALL_SPEED_UP_RIGHT			261

#define ID_ANI_LUIGI_SPEED_UP_LEFT					860
#define ID_ANI_LUIGI_SPEED_UP_RIGHT					861

#define ID_ANI_LUIGI_RACOON_SPEED_UP_LEFT			1570
#define ID_ANI_LUIGI_RACOON_SPEED_UP_RIGHT			1571

// DIE
#define ID_ANI_LUIGI_DIE							998

#pragma endregion


#define GROUND_Y									160.0f

#define	MARIO										100
#define LUIGI										200

#define	MARIO_LEVEL_SMALL							1
#define	MARIO_LEVEL_BIG								2
#define MARIO_LEVEL_RACOON							3

#define MARIO_SMALL_BBOX_WIDTH						13
#define MARIO_SMALL_BBOX_HEIGHT						12

#define MARIO_BIG_BBOX_WIDTH						14
#define MARIO_BIG_BBOX_HEIGHT						24

#define MARIO_BIG_SITTING_BBOX_WIDTH				14
#define MARIO_BIG_SITTING_BBOX_HEIGHT				16

#define MARIO_SIT_HEIGHT_ADJUST						((MARIO_BIG_BBOX_HEIGHT - MARIO_BIG_SITTING_BBOX_HEIGHT) / 2)

#define MARIO_RACOON_BBOX_WIDTH						MARIO_BIG_BBOX_WIDTH
#define MARIO_RACOON_BBOX_HEIGHT					25

#define MARIO_RACOON_SITTING_BBOX_WIDTH				MARIO_BIG_SITTING_BBOX_WIDTH
#define MARIO_RACOON_SITTING_BBOX_HEIGHT			16

#define MARIO_TAIL_WIDTH							10
#define MARIO_TAIL_HEIGHT							5

#define MARIO_TAIL_POSITION_ADJUST					5

#define MARIO_UNTOUCHABLE_TIME						2500
#define MARIO_ATTACK_TIME							300
#define MARIO_KICK_TIME								200
#define MARIO_FLY_TIME								4000
#define MARIO_REFLOAT_TIME							200
#define MARIO_TRANSFORMATION_TIME					600
#define MARIO_DIE_TIME								3000

#define MARIO_GETINTO_PIPE_DOWN						1
#define MARIO_GETINTO_PIPE_UP						2

class CMario : public CGameObject
{

	float ax;				// acceleration on x 
	float ay;				// acceleration on y 
	float maxVx;

	int coin;
	int flag;
	int type;
	int level;
	int holdable;
	int old_level;
	int untouchable;
	int canGetIntoPipe;

	ULONGLONG time_count;
	ULONGLONG untouchable_start;
	ULONGLONG fly_start;
	ULONGLONG float_start;
	ULONGLONG transform_start;

	BOOLEAN isSitting;
	BOOLEAN isOnPlatform;

	CGameObject* _tail;
	CKoopaTroopa* _koopa;
	

	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithKoopaTroopa(LPCOLLISIONEVENT e);
	void OnCollisionWithCoin(LPCOLLISIONEVENT e);
	void OnCollisionWithPortal(LPCOLLISIONEVENT e);
	void OnCollisionWithMushroom(LPCOLLISIONEVENT e);
	void OnCollisionWithBrick(LPCOLLISIONEVENT e);
	void OnCollisionWithPiranhaPlant(LPCOLLISIONEVENT e);

	int GetAniIdBig();
	int GetAniIdSmall();
	int GetAniIdRacoon();

public:
	CMario(float x, float y, int type=100) : CGameObject(x, y)
	{
		maxVx = 0.0f;

		ax = 0.0f;
		ay = MARIO_GRAVITY;
		this->type = type;
		_koopa = NULL;
		_tail = new CTail(x, y);
		untouchable_start = -1;
		level = CHud::GetInstance()->GetLevel();
		isSitting = isOnPlatform = false;
		untouchable = coin = flag = holdable = canGetIntoPipe = 0;
		fly_start = float_start = time_count = transform_start = 0;
	}

	void SetGravity(float gravity) { 
		ay = gravity; 
	}

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);

	int IsCollidable() {
		return (state != MARIO_STATE_DIE);
	}

	int IsHolding() {
		return _koopa != NULL;
	}

	int IsBlocking() { 
		return (state != MARIO_STATE_DIE && untouchable == 0); 
	}

	int IsUntouchable() {
		return untouchable;
	}

	BOOLEAN IsOnPlatform() {
		return isOnPlatform;
	}

	bool IsFlying() { 
		return fly_start != 0; 
	}

	bool IsFloating() { 
		return float_start != 0; 
	}

	void SetTransformStart() { 
		transform_start = GetTickCount64(); 
	}

	bool IsTransforming() { 
		return transform_start != 0;
	}

	void SetCanGetIntoPipe(int var) { 
		canGetIntoPipe = var; 
	}

	int IsCanGetIntoPipe() { 
		return canGetIntoPipe; 
	}

	void SetMaxVx(float vx) {
		maxVx = vx;
	}

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);

	void SetLevel(int l);
	int GetLevel() { return level; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount64(); }

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};
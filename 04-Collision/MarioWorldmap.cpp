#include "Portal.h"
#include "Platform.h"
#include "Collision.h"
#include "MarioWorldmap.h"
#include "PlatformAnimate.h"

void CMarioWorldmap::SetState(int State) {
	state = State;
	switch (state) {
	case MARIO_WORLDMAP_STATE_WALKING_RIGHT:
		vx = MARIO_WORLDMAP_WALKING_SPEED;
		vy = 0;

		nx = 1;
		ny = 0;

		break;

	case MARIO_WORLDMAP_STATE_WALKING_LEFT:
		vx = -MARIO_WORLDMAP_WALKING_SPEED;
		vy = 0;

		nx = -1;
		ny = 0;

		break;

	case MARIO_WORLDMAP_STATE_WALKING_UP:
		vy = -MARIO_WORLDMAP_WALKING_SPEED;
		vx = 0;

		ny = -1;
		nx = 0;

		break;

	case MARIO_WORLDMAP_STATE_WALKING_DOWN:
		vy = MARIO_WORLDMAP_WALKING_SPEED;
		vx = 0;

		ny = 1;
		nx = 0;

		break;
	}
}

void CMarioWorldmap::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = x - MARIO_BBOX_WIDTH / 2;
	top = y - MARIO_BBOX_HEIGHT / 2;
	right = left + MARIO_BBOX_WIDTH;
	bottom = top + MARIO_BBOX_HEIGHT;
}

void CMarioWorldmap::OnNoCollision(DWORD dt) {
	x += vx * dt;
	y += vy * dt;
}

void CMarioWorldmap::OnCollisionWith(LPGAMEOBJECT o) {
	if (canGoIntoPortal && dynamic_cast<CPortal*>(o))
		dynamic_cast<CPortal*>(o)->SwitchScene();
}

void CMarioWorldmap::OnCollisionWith(LPCOLLISIONEVENT e) {
	if (e->obj->IsBlocking()) {
		if (dynamic_cast<CPlatformAnimate*>(e->obj)) {
			CPlatformAnimate* platform = dynamic_cast<CPlatformAnimate*>(e->obj);

			if (platform->GetType() == PLATFORM_ANIMATE_TYPE_GATE)
				absolutelyTouching = 1;
		}

		vx = 0;
		vy = 0;
	}

	if (dynamic_cast<CPortal*>(e->obj))
		if (canGoIntoPortal) {
			DebugOut(L"Collide with portal\n");
			dynamic_cast<CPortal*>(e->obj)->SwitchScene();
		}
}

void CMarioWorldmap::Render() {
	int aniId = ID_ANI_MARIO_SMALL_WORLDMAP;

	switch (level) {
	case MARIO_LEVEL_SMALL:
		aniId = ID_ANI_MARIO_SMALL_WORLDMAP;
		break;

	case MARIO_LEVEL_BIG:
		aniId = ID_ANI_MARIO_BIG_WORLDMAP;
		break;

	case MARIO_LEVEL_RACOON:
		aniId = ID_ANI_MARIO_RACOON_WORLDMAP;
		break;
	}

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
}

void CMarioWorldmap::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	CCollision::GetInstance()->Process(this, coObjects);
	if (absolutelyTouching) {
		if (nx > 0) {
			old_pos = x;
			vx = MARIO_WORLDMAP_WALKING_SPEED;
		}
		else if (nx < 0) {
			old_pos = x;
			vx = -MARIO_WORLDMAP_WALKING_SPEED;
		}
		else if (ny > 0) {
			old_pos = y;
			vy = MARIO_WORLDMAP_WALKING_SPEED;
		}
		else if (ny < 0) {
			old_pos = y;
			vy = -MARIO_WORLDMAP_WALKING_SPEED;
		}
		crossingStart = GetTickCount64();
		absolutelyTouching = 0;
	}

	if (crossingStart) {
		bool adjusted = false;

		if (vx > 0 && (x + vx * dt - old_pos) > MARIO_CROSSING_RANGE) {
			vx = (MARIO_CROSSING_RANGE + old_pos - x) / dt;
			adjusted = true;
		}
		else if (vx < 0 && (old_pos - (x + vx * dt)) > MARIO_BBOX_WIDTH) {
			vx = -(MARIO_BBOX_WIDTH - old_pos + x) / (dt);
			adjusted = true;
		}
		else if (vy > 0 && (y + vy * dt - old_pos) > MARIO_BBOX_HEIGHT) {
			vy = (MARIO_BBOX_HEIGHT + old_pos - y) / dt;
			adjusted = true;
		}
		else if (vy < 0 && (old_pos - (y + vy * dt)) > MARIO_BBOX_HEIGHT) {
			vy = -(MARIO_BBOX_HEIGHT - old_pos + y) / (dt);
			adjusted = true;
		}

		x += vx * dt;
		y += vy * dt;

		if (adjusted) {
			vx = 0;
			vy = 0;
			crossingStart = 0;
		}
	}

	if (crossingStart && GetTickCount64() - crossingStart > MARIO_CROSSING_TIME)
		crossingStart = 0;

	CCollision::GetInstance()->Process(this, dt, coObjects);
}
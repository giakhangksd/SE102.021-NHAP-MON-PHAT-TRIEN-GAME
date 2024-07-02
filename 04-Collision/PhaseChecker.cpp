#include "PhaseChecker.h"

void CPhaseChecker::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = x - width / 2;
	top = y - height / 2;
	right = left + width;
	bottom = top + height;
}

void CPhaseChecker::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	vy += ay * dt;
	vx += ax * dt;

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CPhaseChecker::Render()
{
	RenderBoundingBox();
}

void CPhaseChecker::OnCollisionWith(LPCOLLISIONEVENT e) {
	if (e->ny != 0) 
		vy = 0;
}

void CPhaseChecker::OnNoCollision(DWORD dt) {
	x += vx * dt;
	y += vy * dt;
}
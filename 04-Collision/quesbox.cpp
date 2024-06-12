#include "quesbox.h"

CQuesbox::CQuesbox(float x, float y) :CGameObject(x, y)
{
	bef_x = x;
	bef_y = y;
	SetState(QUESBOX_STATE);
}

void CQuesbox::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - QUESBOX_BBOX_WIDTH / 2;
	t = y - QUESBOX_BBOX_HEIGHT / 2;
	r = l + QUESBOX_BBOX_WIDTH;
	b = t + QUESBOX_BBOX_HEIGHT;
}

void CQuesbox::OnNoCollision(DWORD dt)
{
	//x += vx * dt;
	//y += vy * dt;
};

void CQuesbox::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->obj->IsBlocking()) return;
	if (dynamic_cast<CQuesbox*>(e->obj)) return;

}

void CQuesbox::Render()
{
	int aniId = ID_ANI_QUESBOX;
	if (state == QUESBOX_STATE_NOT)
	{
		aniId = ID_ANI_QUESBOX_NOT;
	}

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CQuesbox::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	y += vy * dt;
	if (y > bef_y) {
		y = bef_y;
		vy = 0;

	}
	if (y < bef_y) {
		vy += 0.009 * dt;
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CQuesbox::SetState(int state) {

	CGameObject::SetState(state);
	switch (state)
	{
	case QUESBOX_STATE:

		break;
	case QUESBOX_STATE_NOT:
		vy = -0.25;
		break;
	}
}



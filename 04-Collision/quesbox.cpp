#include "quesbox.h"

CQuesbox::CQuesbox(float x, float y) :CGameObject(x, y)
{
	SetState(QUESBOX_STATE);
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

void CQuesbox::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - QUESBOX_BBOX_WIDTH / 2;
	t = y - QUESBOX_BBOX_HEIGHT / 2;
	r = l + QUESBOX_BBOX_WIDTH;
	b = t + QUESBOX_BBOX_HEIGHT;
}




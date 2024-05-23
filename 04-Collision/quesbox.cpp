#include "quesbox.h"

void CQuesbox::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_QUESBOX)->Render(x, y);
	
	RenderBoundingBox();
}

void CQuesbox::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - QUESBOX_BBOX_WIDTH / 2;
	t = y - QUESBOX_BBOX_HEIGHT / 2;
	r = l + QUESBOX_BBOX_WIDTH;
	b = t + QUESBOX_BBOX_HEIGHT;
}
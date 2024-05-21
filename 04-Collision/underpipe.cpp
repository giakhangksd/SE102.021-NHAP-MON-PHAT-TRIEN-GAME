#include "underpipe.h"

void Cunderpipe::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANY_UNDERPIPE)->Render(x, y);
}

void Cunderpipe::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - UNDERPIPE_BBOX_WIDTH / 2;
	t = y - UNDERPIPE_BBOX_HEIGHT / 2;
	r = l + UNDERPIPE_BBOX_WIDTH;
	b = t + UNDERPIPE_BBOX_HEIGHT;
}
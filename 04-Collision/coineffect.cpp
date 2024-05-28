#include "Coineffect.h"

void CCoinjump::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_COINJUMP)->Render(x, y);

	//RenderBoundingBox();
}

void CCoinjump::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - COINJUMP_BBOX_WIDTH / 2;
	t = y - COINJUMP_BBOX_HEIGHT / 2;
	r = l + COINJUMP_BBOX_WIDTH;
	b = t + COINJUMP_BBOX_HEIGHT;
}
#include "Header.h"

void Cworldmap::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_WORLDMAP)->Render(x, y);

	//RenderBoundingBox();
}

void Cworldmap::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - COIN_BBOX_WIDTH / 2;
	t = y - COIN_BBOX_HEIGHT / 2;
	r = l + COIN_BBOX_WIDTH;
	b = t + COIN_BBOX_HEIGHT;
}
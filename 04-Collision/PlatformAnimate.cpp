#include "PlatformAnimate.h"

void CPlatformAnimate::Render() {
	if (isAni) CAnimations::GetInstance()->Get(aniOrsprite)->Render(x, y);
	else CSprites::GetInstance()->Get(aniOrsprite)->Draw(x, y);
}

void CPlatformAnimate::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = x - PLATFORM_ANIMATE_BBOX_WIDTH / 2;
	top = y - PLATFORM_ANIMATE_BBOX_HEIGHT / 2;
	right = left + PLATFORM_ANIMATE_BBOX_WIDTH;
	bottom = top + PLATFORM_ANIMATE_BBOX_HEIGHT;
}

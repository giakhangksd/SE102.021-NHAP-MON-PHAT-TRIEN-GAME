#include "debug.h"
#include "Effect.h"
#include "AssetIDs.h"

CEffect* CEffect::_instance = NULL;

void CEffect::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = x;
	top = y;
	right = left;
	bottom = top;
}

void CEffect::Render() {
	for (LPGAMEOBJECT e : effects) {
		CEffect* effect = dynamic_cast<CEffect*>(e);
		effect->SpecificRender();
	}
}

void CEffect::SpecificRender() {
	if (render_start) {
		if (isSprite) CSprites::GetInstance()->Get(idSpriteOrAni)->Draw(x, y);
		else CAnimations::GetInstance()->Get(idSpriteOrAni)->Render(x, y);

		if (GetTickCount64() - render_start > EFFECT_TIME) {
			render_start = 0;
			vy = 0;
			isDeleted = true;
		}
	}
}

void CEffect::SetPosition(float px, float py) {
	x = px;
	y = py;
	render_start = GetTickCount64();
}

void CEffect::Deflected(int Direction) {
	vy = -EFFECT_SPEED;
}

void CEffect::PurgeDeletedEffects() {
	vector<LPGAMEOBJECT>::iterator it;
	for (it = effects.begin(); it != effects.end(); it++) {
		LPGAMEOBJECT o = *it;
		if (o->IsDeleted()) {
			delete o;
			*it = NULL;
		}
	}

	effects.erase(std::remove_if(effects.begin(), effects.end(), CEffect::IsEffectDeleted), effects.end());
}

bool CEffect::IsEffectDeleted(const LPGAMEOBJECT& e) {
	return e == NULL;
}
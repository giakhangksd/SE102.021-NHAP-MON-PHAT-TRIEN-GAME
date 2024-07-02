#pragma once
#include "GameObject.h"

#define EFFECT_SPEED			0.04f

#define EFFECT_TIME				500

class CEffect : public CGameObject {
protected:
	static CEffect* _instance;
	vector<LPGAMEOBJECT> effects;

	ULONGLONG render_start;
	
	int idSpriteOrAni;
	bool isSprite;

public:
	CEffect() {
		render_start = 0;
		vy = 0;
		effects.clear();
		idSpriteOrAni = 0;
		isSprite = true;
	}

	static CEffect* GetInstance() {
		if (_instance == NULL)
			_instance = new CEffect();
		return _instance;
	}

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	virtual void Update(DWORD dt) {
		for (LPGAMEOBJECT e : effects) {
			CEffect* effect = dynamic_cast<CEffect*>(e);
			effect->y += (effect->vy * dt);
		}
	}

	void Render();
	void SpecificRender();

	void SetPosition(float px, float py);

	virtual int IsBlocking() {
		return 0; 
	}

	virtual void Deflected(int Direction = 0);

	void pushEffectIntoQueue(float px, float py, int IdSpriteorAni, bool IsSprite = 1, bool IsDeflect = 0) {
		LPGAMEOBJECT obj = new CEffect();
		CEffect* effect = dynamic_cast<CEffect*>(obj);

		effect->idSpriteOrAni = IdSpriteorAni;
		effect->isSprite = IsSprite;
		effect->SetPosition(px, py);

		if (IsDeflect)
			effect->Deflected();
		effects.push_back(effect);
	}

	void PurgeDeletedEffects();

	static bool IsEffectDeleted(const LPGAMEOBJECT& e);
};

typedef CEffect* LPEFFECT;


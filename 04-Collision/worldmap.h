#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_WORLDMAP 11020

#define	COIN_WIDTH 10
#define COIN_BBOX_WIDTH 10
#define COIN_BBOX_HEIGHT 16

class Cworldmap : public CGameObject {
public:
	Cworldmap(float x, float y) : CGameObject(x, y) {}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
};
#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "AssetIDs.h"

#define ID_ANI_BRICK_BROKEN 11011

#define BROKEN_SPEED_Y	0.1f
#define BROKEN_SPEED_X	0.03f

class CBrickBroken : public CGameObject {
public:
	CBrickBroken(float x, float y, int nx, float vy);
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* object);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) {};
	virtual int IsBlockingX() { return 0; };
	virtual int IsBlockingY() { return 0; };
};
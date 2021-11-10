#pragma once
#include "GameObject.h"

#define ID_ANI_FUNNEL 12001

#define FUNNEL_BBOX_WIDTH  32
#define FUNNEL_BBOX_HEIGHT 48

class CFunnel : public CGameObject
{
public:
	CFunnel(float x, float y) : CGameObject(x, y) {}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

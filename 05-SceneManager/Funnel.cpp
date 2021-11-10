#include "Funnel.h"

void CFunnel::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_FUNNEL)->Render(x, y);

	//RenderBoundingBox();
}

void CFunnel::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - FUNNEL_BBOX_WIDTH / 2;
	t = y - FUNNEL_BBOX_HEIGHT / 2;
	r = l + FUNNEL_BBOX_WIDTH;
	b = t + FUNNEL_BBOX_HEIGHT;
}
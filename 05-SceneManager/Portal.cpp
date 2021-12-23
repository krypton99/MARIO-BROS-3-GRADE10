#include "Portal.h"
#include "Game.h"
#include "Textures.h"
#include "AssetIDs.h"

CPortal::CPortal(float l, float t, float r, float b, int scene_id, int portal_type, float player_x, float player_y)
{
	this->scene_id = scene_id;
	x = l;
	y = t;
	width = r - l;
	height = b - t;
	type = OBJECT_TYPE_PORTAL;
	this->portal_type = portal_type;
	this->player_x = player_x;
	this->player_y = player_y;
}

void CPortal::RenderBoundingBox()
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPTEXTURE bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);

	float l, t, r, b;

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	float cx, cy;
	CGame::GetInstance()->GetCamPos(cx, cy);

	CGame::GetInstance()->Draw(x - cx, y - cy, bbox, nullptr, BBOX_ALPHA, rect.right - 1, rect.bottom - 1);
}

void CPortal::Render()
{
	RenderBoundingBox();
}

void CPortal::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - width / 2;
	t = y - height / 2;
	r = x + width / 2;
	b = y + height / 2;
}
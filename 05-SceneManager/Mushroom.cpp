#include "Mushroom.h"
#include "GameObject.h"
#include "Mario.h"
#define BRICK_BOUNDING_X 5.0f
CMushroom::CMushroom(float x, float y, float mushroomType) 
{
	this->x = x;
	this->y = y;
	ItemType = mushroomType;
	vx = -ITEM_SPEED_Y;
	this->ax = 0;
	this->ay = ITEM_GRAVITY;
}
void CMushroom::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CMushroom::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlockingX() && !e->obj->IsBlockingY()) return;
	if (dynamic_cast<CMushroom*>(e->obj)) return;

	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
	/*if (dynamic_cast<CMario*>(e->obj))
		OnCollisionWithMario(e);*/
}
void CMushroom::OnCollisionWithMario(LPCOLLISIONEVENT e)
{
	CMario* mario = dynamic_cast<CMario*>(e->obj);
	if (ItemType == ITEM_RED_MUSHROOM) {
		if (mario->GetLevel() > MARIO_LEVEL_SMALL)
		{
			e->obj->SetState(STATE_ERASE);
			return;
		}
		else
		{
			e->obj->SetState(STATE_ERASE);
			mario->SetLevel(MARIO_LEVEL_BIG);

		}
	}
}
void CMushroom::Render()
{

	CAnimations* animations = CAnimations::GetInstance();
	int ani = ID_ITEM_ANI_MUSHROOM;
	
	animations->Get(ani)->Render(x, y);
	RenderBoundingBox();
}

void CMushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	Item::Update(dt,coObjects);
	/*this->vy += (ITEM_GRAVITY * dt);
	if (state == BRICK_STATE_BOUND) {
		if (y < (start_y - BRICK_BOUNDING_X) && vy < 0) {
			vy = -vy;
		}
		if (y > start_y) {
			y = start_y;
			SetState(BRICK_STATE_EMPTY);
		}
	}
	y += vy * dt;*/
	CGameObject::Update(dt, coObjects);
	vy += ay * dt;
	vx += ax * dt;
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

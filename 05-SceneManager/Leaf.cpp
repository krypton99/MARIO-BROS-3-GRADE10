#include "Leaf.h"
#include "GameObject.h"
#include "Mario.h"
#include "Grid.h"
#define BRICK_BOUNDING_X 5.0f
CLeaf::CLeaf(float x, float y, float leafType)
{
	this->x = x;
	this->y = y;
	ItemType = leafType;
	vx = -ITEM_SPEED_Y;
	this->ax = 0;
	this->ay = ITEM_GRAVITY;
	start_x = x;
}
void CLeaf::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CLeaf::OnCollisionWith(LPCOLLISIONEVENT e)
{
	//if (!e->obj->IsBlockingX() && !e->obj->IsBlockingY()) return;
	if (dynamic_cast<CLeaf*>(e->obj)) return;

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
void CLeaf::OnCollisionWithMario(LPCOLLISIONEVENT e)
{
	CMario* mario = dynamic_cast<CMario*>(e->obj);
	if (ItemType == ITEM_LEAF) {
		if (mario->GetLevel() > MARIO_LEVEL_BIG)
		{
			e->obj->SetState(STATE_ERASE);
			return;
		}
		else
		{
			e->obj->SetState(STATE_ERASE);
			mario->SetLevel(MARIO_LEVEL_RACOON);

		}
	}
}
void CLeaf::Render()
{

	CAnimations* animations = CAnimations::GetInstance();
	int ani;
	if (vx > 0) {  ani = ID_ITEM_ANI_LEAF_RIGHT; }
	else  ani = ID_ITEM_ANI_LEAF_LEFT;

	animations->Get(ani)->Render(x, y);
	RenderBoundingBox();
}

void CLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	Item::Update(dt, coObjects);
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
	if (!isFalling)
		vy += (ITEM_GRAVITY * dt);
	else
		vy = LEAF_SPEED_FALL;

	
	if (!isFalling)
	{
		isFalling = true;
		vx = -LEAF_SPEED_X;
	}

	if (isFalling)
	{
		if (vx < 0 && x < start_x-BRICK_BBOX)
		{
			vx = -vx;
			x = start_x - BRICK_BBOX;
		}
		if (vx > 0 && x > start_x + BRICK_BBOX)
		{
			vx = -vx;
			x = start_x + BRICK_BBOX;
		}
	}
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

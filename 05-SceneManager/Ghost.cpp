#include "Ghost.h"

CGhost::CGhost(float x, float y) :CGameObject(x, y)
{
	this->ax = 0;
	this->ay = GHOST_GRAVITY;
	die_start = -1;
	SetState(GHOST_STATE_STICK);
	start_x = x;
	start_y = y;
}

void CGhost::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	
	{
		left = x - GHOST_BBOX_WIDTH / 2;
		top = y - GHOST_BBOX_HEIGHT / 2;
		right = left + GHOST_BBOX_WIDTH;
		bottom = top + GHOST_BBOX_HEIGHT;
	}
}

void CGhost::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CGhost::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlockingX()&&!e->obj->IsBlockingY()) return;
	if (dynamic_cast<CGhost*>(e->obj)) return;

	if (e->ny != 0)
	{
		vy = 0;
		if (e->ny < 0) // va cham ground
		{
			isOnGround = true;

		}
	}
	else if (e->nx != 0)
	{
		vx = -vx;

	}
	
}

void CGhost::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	
	vy += ay * dt;
	vx += ax * dt;
	//CGameObject::Update(dt, coObjects);
	isOnGround = false;
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CGhost::Render()
{
	RenderBoundingBox();
}

void CGhost::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GHOST_STATE_STICK:
		vx = 0.05f;
		break;
	}
}



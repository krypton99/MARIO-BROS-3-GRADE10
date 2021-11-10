#include "Koopas.h"

CKoopas::CKoopas(float x, float y, float type) : CGameObject(x, y)
{
	//this->ax = 0;
	this->ay = TROOPA_GRAVITY ;
	this->type = type;
	die_start = -1;
	SetState(TROOPA_STATE_WALKING);
}

void CKoopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == TROOPA_STATE_DIE)
	{
		left = x - TROOPA_BBOX_WIDTH / 2;
		top = y - TROOPA_BBOX_HEIGHT_DIE / 2;
		right = left + TROOPA_BBOX_WIDTH;
		bottom = top + TROOPA_BBOX_HEIGHT_DIE;
	}
	else
	{
		left = x - TROOPA_BBOX_WIDTH / 2;
		top = y - TROOPA_BBOX_HEIGHT / 2;
		right = left + TROOPA_BBOX_WIDTH;
		bottom = top + TROOPA_BBOX_HEIGHT;
	}
}

void CKoopas::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CKoopas::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CKoopas*>(e->obj)) return;

	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	if (state == TROOPA_STATE_ROLL)
	{
		
		if (nx > 0) {
			vx = vx;
		}
		else if (nx < 0)
		{
			vx = -vx;
		}
		vy += ay * dt;
	}
	else {
		vy += ay * dt;
		vx += ax * dt;
	}
	/*if ((state == TR_STATE_DIE) && (GetTickCount64() - die_start > GOOMBA_DIE_TIMEOUT))
	{
		isDeleted = true;
		return;
	}*/

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CKoopas::Render()
{
	if (type == KOOPAS_TYPE_RED) {
		
		int	aniId = ID_ANI_RED_TROOPA_WALKING_LEFT;
		
		//int ani = KOOPA_TROOPA_ANI_WALKING_LEFT;
		if (state == TROOPA_STATE_DIE && vx == 0) {
			aniId = ID_ANI_RED_TROOPA_DIE_DOWN_IDLE;
		}
		else if (state == TROOPA_STATE_ROLL) {
			aniId = ID_ANI_RED_TROOPA_DIE_DOWN_RUN;
		}
		else if (vx > 0) aniId = ID_ANI_RED_TROOPA_WALKING_RIGHT;
		else if (vx < 0) aniId = ID_ANI_RED_TROOPA_WALKING_LEFT;
		CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	}
	else if (type == KOOPAS_TYPE_GREEN_WING) {
		
			int aniId = 0;
			if (vx > 0) {
				aniId = ID_ANI_GREEN_TROOPA_FLY_RIGHT;
			}
			else if(vx<0){
				aniId = ID_ANI_GREEN_TROOPA_FLY_LEFT;
			}
			CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	} else if(type == KOOPAS_TYPE_GREEN){
		int aniId = ID_ANI_GREEN_TROOPA_WALKING_LEFT;
		//int ani = KOOPA_TROOPA_ANI_WALKING_LEFT;
		if (state == TROOPA_STATE_DIE && vx == 0) {
			aniId = ID_ANI_GREEN_TROOPA_DIE_DOWN_IDLE;
		}
		else if (state == TROOPA_STATE_ROLL) {
			aniId = ID_ANI_GREEN_TROOPA_DIE_DOWN_RUN;
		}
		else if (vx > 0) aniId = ID_ANI_GREEN_TROOPA_WALKING_RIGHT;
		else if (vx < 0) aniId = ID_ANI_GREEN_TROOPA_WALKING_LEFT;
		CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	}
	
	//RenderBoundingBox();
}

void CKoopas::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case TROOPA_STATE_DIE:
		y += ((TROOPA_BBOX_HEIGHT - TROOPA_BBOX_HEIGHT_DIE) / 2)-5;
		vx = 0;
		vy = 0;
		ay = 0;
		break;
	case TROOPA_STATE_WALKING:
		vx = TROOPA_WALKING_SPEED;
		break;
	case TROOPA_STATE_ROLL:
		vx = TROOPA_ROLLING_SPEED;
	}
}

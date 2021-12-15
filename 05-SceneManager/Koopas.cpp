#include "Koopas.h"
#include "AssetIDs.h"
#include "Goomba.h"
#include "Mario.h"
#include "Platform.h"
#include "Brick.h"
#include "PlayScene.h"

CKoopas::CKoopas(float x, float y, float type) : CGameObject(x, y)
{
	this->ax = 0;
	this->ay = TROOPA_GRAVITY ;
	this->koopa_type = type;
	this->type = OBJECT_TYPE_TROOPA;
	timeStartJump->Start();
	type = OBJECT_TYPE_KOOPAS;
	die_start = -1;
	SetState(TROOPA_STATE_DIE);
	start_vx = vx;
	isGhostFollow = true;
	/*ghost = new CGhost(x+16, y);*/
	//ghost->Render();
}

void CKoopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == TROOPA_STATE_DIE||  state == TROOPA_STATE_ROLL_LEFT || state == TROOPA_STATE_ROLL_RIGHT)
	{
		left = x - TROOPA_BBOX_WIDTH/2;
		top = y - TROOPA_BBOX_HEIGHT_DIE/2;
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
	DebugOut(L"no collision");
	x += vx * dt;
	y += vy * dt;
};

void CKoopas::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (state == TROOPA_STATE_ROLL_LEFT || state == TROOPA_STATE_ROLL_RIGHT) {
		if (dynamic_cast<CGoomba*>(e->obj))
			OnCollisionWithGoomba(e);
	}
	/*if (state != TROOPA_STATE_ROLL) {*/

		if (!e->obj->IsBlockingX() && !e->obj->IsBlockingY()) return;
		if (dynamic_cast<CKoopas*>(e->obj)) return;
	
	/*}*/
	
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
	if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	/*else 
	if (dynamic_cast<CBrick*>(e->obj))
		if (type == KOOPAS_TYPE_RED) { OnCollisionWithBrick(e); }*/
}
void CKoopas::OnCollisionWithGoomba(LPCOLLISIONEVENT e) {
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
	if (state == TROOPA_STATE_ROLL_LEFT || state == TROOPA_STATE_ROLL_RIGHT) {
		goomba->SetState( GOOMBA_STATE_DIE_BY_OBJECT);
	}

}
void CKoopas::OnCollisionWithBrick(LPCOLLISIONEVENT e) {
	//vx = -vx;
	/*CBrick* brick = dynamic_cast<CBrick*>(e->obj);
	if (state == TROOPA_STATE_ROLL_LEFT || state == TROOPA_STATE_ROLL_RIGHT) {
		if (brick->GetBrickType() == BRICK_TYPE_HIDDEN) {
			brick->isBlocking = 0;
		}
	}*/
	/*else brick->isBlocking = 1;*/
}
//void CKoopas::OnCollisionWithGoomba(LPCOLLISIONEVENT e) {
//	CMario* goomba = dynamic_cast<CMario*>(e->obj);
//	if (state == TROOPA_STATE_ROLL) {
//		goomba->SetState(GOOMBA_STATE_DIE_BY_OBJECT);
//	}
//
//}
void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	
	vy += ay * dt;
	vx += ax * dt;
	if (isMariohold) {
		ay = 0;
	}
	else ay = TROOPA_GRAVITY;
	if (koopa_type == KOOPAS_TYPE_GREEN_WING && state != TROOPA_STATE_DIE) {
		if (timeStartJump->IsTimeUp() && timeStartJump->GetStartTime()) { // bd tinh time nhay
			timeStartJump->Stop();
			SetState(TROOPA_STATE_JUMP);
			timeStartJump->Start();
		}
	}
	if (ghost->isOnGround == false && (state!=TROOPA_STATE_DIE && state!=TROOPA_STATE_ROLL_LEFT && state!=TROOPA_STATE_ROLL_RIGHT)) {
		vx = -vx;
		if (vx > 0) {
			ghost->SetSpeed(vx, vy);
			ghost->SetPos(this->x + 17, y+4);
		}
		else if (vx < 0) {
			ghost->SetSpeed(vx, vy);
			ghost->SetPos(this->x - 17, y+4);
		}
	}
	//CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CKoopas::Render()
{
	if (koopa_type == KOOPAS_TYPE_RED) {
		
		int	aniId = ID_ANI_RED_TROOPA_WALKING_LEFT;
		
		//int ani = KOOPA_TROOPA_ANI_WALKING_LEFT;
		if (state == TROOPA_STATE_DIE && vx == 0) {
			aniId = ID_ANI_RED_TROOPA_DIE_DOWN_IDLE;
		}
		else if (state == TROOPA_STATE_ROLL_LEFT || state == TROOPA_STATE_ROLL_RIGHT) {
			aniId = ID_ANI_RED_TROOPA_DIE_DOWN_RUN;
		}
		else if (vx > 0) aniId = ID_ANI_RED_TROOPA_WALKING_RIGHT;
		else if (vx < 0) aniId = ID_ANI_RED_TROOPA_WALKING_LEFT;
		CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	}
	else if (koopa_type == KOOPAS_TYPE_GREEN_WING) {
		
			int aniId = 0;
			if (vx > 0) {
				aniId = ID_ANI_GREEN_TROOPA_FLY_RIGHT;
			}
			else if(vx<0){
				aniId = ID_ANI_GREEN_TROOPA_FLY_LEFT;
			}
			CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	} else if(koopa_type == KOOPAS_TYPE_GREEN){
		int aniId = ID_ANI_GREEN_TROOPA_WALKING_LEFT;
		//int ani = KOOPA_TROOPA_ANI_WALKING_LEFT;
		if (state == TROOPA_STATE_DIE && vx == 0) {
			aniId = ID_ANI_GREEN_TROOPA_DIE_DOWN_IDLE;
		}
		else if (state == TROOPA_STATE_ROLL_LEFT || state== TROOPA_STATE_ROLL_RIGHT) {
			aniId = ID_ANI_GREEN_TROOPA_DIE_DOWN_RUN;
		}
		else if (vx > 0) aniId = ID_ANI_GREEN_TROOPA_WALKING_RIGHT;
		else if (vx < 0) aniId = ID_ANI_GREEN_TROOPA_WALKING_LEFT;
		CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	}
	
	RenderBoundingBox();
}

void CKoopas::SetState(int state)
{
	if (this->state == TROOPA_STATE_ROLL_LEFT || this->state==TROOPA_STATE_ROLL_RIGHT ) {
		return;
	}
	CGameObject::SetState(state);
	
	switch (state)
	{
	case TROOPA_STATE_DIE:
		y += ((TROOPA_BBOX_HEIGHT - TROOPA_BBOX_HEIGHT_DIE) / 2)-20;
		vx = 0;
		vy = 0;
		//ay = 0;
		//timeStartJump->Stop();
		break;
	case TROOPA_STATE_WALKING:
		vx = TROOPA_WALKING_SPEED;
		break;
	case TROOPA_STATE_ROLL_LEFT:
		vx = -TROOPA_ROLLING_SPEED;
		break;
	case TROOPA_STATE_ROLL_RIGHT:
		vx = TROOPA_ROLLING_SPEED;
		break;
	case TROOPA_STATE_JUMP:
		isOnGround = false;
		vy = -TROOPA_JUMP_SPEED;
		break;
	}
}

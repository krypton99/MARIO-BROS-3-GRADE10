#include "Tail.h"
#include "GameObject.h"
#include "AssetIDs.h"
#include "Mario.h"
#include "Koopas.h"
#include "Goomba.h"
#include "Brick.h"

CTail* CTail::__instance = nullptr;

CTail* CTail::GetInstance(float x, float y)
{
	if (__instance == NULL) __instance = new CTail(x,y);
	return __instance;
}
CTail::CTail(float x, float y) :CGameObject(x, y)
{
	this->start_x = x;
	this->start_y = y;
}
void CTail::Render()
{
	/*if (brickType != BRICK_TYPE_HIDDEN) {
		
		int ani = ID_ANI_QUESTION_BRICK_ACTIVE;
		if (state == BRICK_STATE_EMPTY) {
			ani = ID_ANI_QUESTION_BRICK_EMPTY;
		}
		else ani = ID_ANI_QUESTION_BRICK_ACTIVE;
		animations->Get(ani)->Render(x, y);
	}*/
	/*CAnimations* animations = CAnimations::GetInstance();
	int ani = ID_ANI_GOOMBA_DIE;
	animations->Get(ani)->Render(x, y);*/
	RenderBoundingBox();
}

void CTail::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (ani) {
		CAnimations* animations = CAnimations::GetInstance();
		int frame = 0;
		if (animations->Get(ani)->getCurrentFrame() != NULL) {
			frame = animations->Get(ani)->getCurrentFrame();
		}
		if (frame == 3 && isAttack) {
			if (nx > 0) {
				l = x + TAIL_BBOX_WIDTH / 2 ;
			}
			else 
				l = x - TAIL_BBOX_WIDTH / 2 ;
		}
		else
		if (frame == 1 && isAttack) {
				if (nx > 0) {
					l = x - TAIL_BBOX_WIDTH / 2;
				}
				else l = x + TAIL_BBOX_WIDTH / 2;
		}
		else l = x - TAIL_BBOX_WIDTH / 2;
		t = y - TAIL_BBOX_HEIGHT / 2;
		r = l + TAIL_BBOX_WIDTH;
		b = t + TAIL_BBOX_HEIGHT;
	}
}
void CTail::OnNoCollision(DWORD dt)
{
	/*x += vx * dt;
	y += vy * dt;*/
}
void CTail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, float x, float y, int nx) {
	CGameObject::Update(dt);
	if (ani) {
		CAnimations* animations = CAnimations::GetInstance();
		int frame = 0;
		if (animations->Get(ani)->getCurrentFrame() != NULL) {
			frame = animations->Get(ani)->getCurrentFrame();
		}
		if (frame == 3 && isAttack) {
			if (nx > 0) {
				this->x = x + MARIO_RACOON_BBOX_WIDTH / 2;
			}
			else
				this->x = x - MARIO_RACOON_BBOX_WIDTH / 2;
		}
		/*else
			if (frame == 1 && isAttack) {
				if (nx > 0) {
					this->x = x - MARIO_RACOON_BBOX_WIDTH / 2;
				}
				else this->x = x + MARIO_RACOON_BBOX_WIDTH / 2;
			}*/
			else {
				if (nx > 0) {
					this->x = x - MARIO_RACOON_BBOX_WIDTH / 2;
				}
				else this->x = x + MARIO_RACOON_BBOX_WIDTH / 2;
			}
	}
	
	this->y = y + MARIO_RACOON_BBOX_HEIGHT/4;
	this->nx = nx;
	CCollision::GetInstance()->Process(this, dt, coObjects);
}
void CTail::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CGoomba*>(e->obj) && isAttack) {
		OnCollisionWithGoomba(e);
	}
	if (dynamic_cast<CBrick*>(e->obj) ) {
		OnCollisionWithBrick(e);
	}
	if (dynamic_cast<CKoopas*>(e->obj) && isAttack) {
		OnCollisionWithKoopas(e);
	}

}
void CTail::OnCollisionWithGoomba(LPCOLLISIONEVENT e) {
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
	if (goomba->GetType() == GOOMBA_TYPE_RED_WING) {
			if (goomba->GetState() != GOOMBA_STATE_DIE)
			{
				goomba->SetType(GOOMBA_TYPE_RED);
			}
	}
	else 
	{
		goomba->SetState(GOOMBA_STATE_DIE_BY_OBJECT);
	}
}
void CTail::OnCollisionWithKoopas(LPCOLLISIONEVENT e) {
	CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);
	koopas->n = this->nx;
	float x, y;
	koopas->GetPosition(x, y);
	koopas->temp_x = x;
	koopas->SetState(TROOPA_STATE_DIE_UP);
}
void CTail::OnCollisionWithBrick(LPCOLLISIONEVENT e) {
	CBrick* brick = dynamic_cast<CBrick*>(e->obj);
	if (brick->GetBrickType() == BRICK_TYPE_GOLD) {
		brick->SetState(BRICK_STATE_BROKEN);
	}
}
void CTail::SetState(int state) {
	CGameObject::SetState(state);
}
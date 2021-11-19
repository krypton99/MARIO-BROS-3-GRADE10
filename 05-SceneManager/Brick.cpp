#include "Brick.h"
#include "GameObject.h"
#include "AssetIDs.h"
#include "Mario.h"
#include "Koopas.h"
#define BRICK_BOUNDING_X 5.0f

CBrick::CBrick(float x, float y, float brickType,float itemType) :CGameObject(x, y)
{
	this->type = OBJECT_TYPE_BRICK;
	this->brickType = brickType;
	this->start_x = x;
	this->start_y = y;
	this->itemType = itemType;
	SetState(BRICK_STATE_ACTIVE);
}
void CBrick::Render()
{
	if (brickType != BRICK_TYPE_HIDDEN) {
		CAnimations* animations = CAnimations::GetInstance();
		int ani = ID_ANI_QUESTION_BRICK_ACTIVE;
		if (state == BRICK_STATE_EMPTY) {
			ani = ID_ANI_QUESTION_BRICK_EMPTY;
		}
		else ani = ID_ANI_QUESTION_BRICK_ACTIVE;
		animations->Get(ani)->Render(x, y);
	}
	//RenderBoundingBox();
}

void CBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x - BRICK_BBOX_WIDTH/2;
	t = y - BRICK_BBOX_HEIGHT/2;
	r = l + BRICK_BBOX_WIDTH;
	b = t + BRICK_BBOX_HEIGHT;
}
void CBrick::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}
void CBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	//CGameObject::Update(dt);
	if (state == BRICK_STATE_BOUND) {
		if (y < (start_y - BRICK_BOUNDING_X) && vy < 0) {
			vy = -vy;
		}
		if (y > start_y) {
			y = start_y;
			SetState(BRICK_STATE_EMPTY);
		}		
	}
	y += vy * dt;
	CCollision::GetInstance()->Process(this, dt, coObjects);
}
void CBrick::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CMario*>(e->obj)) {
		//isBlocking = 0;
		/*CBrick* brick = dynamic_cast<CBrick*>(e->obj);*/
		//isBlocking = 0;
	}
	else 
	if (dynamic_cast<CKoopas*>(e->obj)) {
		CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);
		if (koopas->GetState() == TROOPA_STATE_ROLL_LEFT || koopas->GetState() == TROOPA_STATE_ROLL_RIGHT) {
			if (brickType == BRICK_TYPE_HIDDEN) {
				isBlocking = 0;
			}
		}
		else isBlocking = 1;
	} else isBlocking = 1;

}
void CBrick::SetState(int state) {
	CGameObject::SetState(state);
	switch (state) {
	case BRICK_STATE_BOUND:
	{
		if (itemType==CONTAIN_MUSHROOM) {
			isFallingItem = true;
		}
		else
			isFallingItem = false;
		vy = -0.1f;
	}
	break;

	case BRICK_STATE_ACTIVE:
		vy = 0;
		break;
	case BRICK_STATE_EMPTY:
		vy = 0;
		break;
	}

}
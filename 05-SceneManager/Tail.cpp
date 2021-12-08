#include "Tail.h"
#include "GameObject.h"
#include "AssetIDs.h"
#include "Mario.h"
#include "Koopas.h"

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
		CAnimations* animations = CAnimations::GetInstance();
		int ani = ID_ANI_QUESTION_BRICK_ACTIVE;
		if (state == BRICK_STATE_EMPTY) {
			ani = ID_ANI_QUESTION_BRICK_EMPTY;
		}
		else ani = ID_ANI_QUESTION_BRICK_ACTIVE;
		animations->Get(ani)->Render(x, y);
	}*/
	RenderBoundingBox();
}

void CTail::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - TAIL_BBOX_WIDTH / 2;
	t = y - TAIL_BBOX_HEIGHT / 2;
	r = l + TAIL_BBOX_WIDTH;
	b = t + TAIL_BBOX_HEIGHT;
}
void CTail::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}
void CTail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, float x, float y) {
	//CGameObject::Update(dt);
	this->x = x;
	this->y = y;
	
	CCollision::GetInstance()->Process(this, dt, coObjects);
}
void CTail::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CMario*>(e->obj)) {
		//isBlocking = 0;
		/*CBrick* brick = dynamic_cast<CBrick*>(e->obj);*/
		//isBlocking = 0;
	}
	

}
void CTail::SetState(int state) {
	CGameObject::SetState(state);
}
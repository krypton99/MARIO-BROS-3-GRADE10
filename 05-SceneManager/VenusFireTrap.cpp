#include "VenusFireTrap.h"
#include "Utils.h"
#include "GameObject.h"
#include "AnimationSet.h"
#include "debug.h"
#include "AssetIDs.h"

CVenusFireTrap::CVenusFireTrap(float x, float y) :CGameObject(x, y)
{
	this->ax = 0;
	hiddenTimer = new Timer(HIDDEN_TIME);
	attackTimer = new Timer(ATTACK_TIME);
	GetPosition(this->start_x, this->start_y);
	SetState(VENUS_STATE_UP);
	type = OBJECT_TYPE_VENUS;
}

void CVenusFireTrap::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - VENUS_BBOX_WIDTH / 2;
	top = y - VENUS_BBOX_HEIGHT / 2;
	right = left + VENUS_BBOX_WIDTH;
	bottom = top + VENUS_BBOX_HEIGHT;
}

void CVenusFireTrap::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CVenusFireTrap::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CVenusFireTrap*>(e->obj)) return;

	/*if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}*/
}

void CVenusFireTrap::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//CGameObject::Update(dt);
	this->y += vy*dt;
	vy += ay * dt;
	vx += ax * dt;
	if (GetState() == VENUS_STATE_UP && y < start_y - VENUS_BBOX_HEIGHT) {
		y = start_y - VENUS_BBOX_HEIGHT; // vi tri y tang len maximum ~ flower height
		SetState(VENUS_STATE_ATTACK);
	}
	if (attackTimer->GetStartTime() != 0 && attackTimer->IsTimeUp()) {
		attackTimer->Stop();
		SetState(VENUS_STATE_DOWN);
	}
	// step 3: (move down)  -> het attackTimer 
	if (GetState() == VENUS_STATE_DOWN && y > start_y) {
		y = start_y + 1;
		SetState(VENUS_STATE_HIDDEN);
	}

	// step 4: (hidden)  -> het hiddenTimer -> moving up (step 1) 
	if (hiddenTimer->GetStartTime() != 0 && hiddenTimer->IsTimeUp()) {
		hiddenTimer->Stop();
		SetState(VENUS_STATE_UP);
	}
	//DebugOut(L"[ERROR] state ID %ld \n", hiddenTimer->GetEndTime());
	//DebugOut(L"[ERROR] state ID %ld \n", IsCollidable());
	
	//else SetState(VENUS_STATE_DOWN);
	/*if ((state == GOOMBA_STATE_DIE) && (GetTickCount64() - die_start > GOOMBA_DIE_TIMEOUT))
	{
		isDeleted = true;
		return;
	}*/

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CVenusFireTrap::Render()
{
	int aniId = 0;
	/*if (state == GOOMBA_STATE_DIE)
	{
		aniId = ID_ANI_GOOMBA_DIE;
	}*/

	CAnimations::GetInstance()->Get(ID_ANI_REDVENUS_MOVE)->Render(x, y);
	//RenderBoundingBox();
}

void CVenusFireTrap::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case VENUS_STATE_ATTACK:
		vy = 0;
		attackTimer->Start();
		break;
	case VENUS_STATE_HIDDEN: 
		vy = 0;
		hiddenTimer->Start();
		break;
	case VENUS_STATE_UP:
		vy = -VENUS_MOVING_SPEED;
		break;
	case VENUS_STATE_DOWN:
		vy = VENUS_MOVING_SPEED;
		break;
	}
}

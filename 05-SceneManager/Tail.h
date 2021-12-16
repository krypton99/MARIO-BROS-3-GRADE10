#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"


#define TAIL_STATE_ATTACK 1

#define TAIL_BBOX_WIDTH		10
#define TAIL_BBOX_HEIGHT	6

class CTail : public CGameObject {
private:
	static CTail* __instance;
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	virtual void OnCollisionWithBrick(LPCOLLISIONEVENT e);
public:
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, float x, float y, float nx);
	CTail(float x, float y);
	void Render();
	virtual int IsCollidable() { return 1; };
	virtual int IsBlockingX() { return 1; };
	virtual int IsBlockingY() { return 1; };
	virtual void SetState(int state);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	static CTail* GetInstance(float x, float y);
	BOOLEAN isAttack = false;
	int ani;
};
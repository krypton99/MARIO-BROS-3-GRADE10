#pragma once
#include "GameObject.h"

#define GHOST_GRAVITY 0.002f
#define GHOST_STATE_STICK 100

#define GHOST_BBOX_WIDTH 16
#define GHOST_BBOX_HEIGHT 14
#define GHOST_BBOX_HEIGHT_DIE 7

#define ID_ANI_GOOMBA_WALKING 5000
#define ID_ANI_GOOMBA_DIE 5001

class CGhost : public CGameObject
{
protected:
	float ax;
	float ay;

	ULONGLONG die_start;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	

	virtual int IsCollidable() { return 1; };
	virtual int IsBlockingX() { return 0; }
	virtual int IsBlockingY() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CGhost(float x, float y);
	void SetPos(float x, float y) { this->x = x; this->y = y; }
	virtual void SetState(int state);
	bool isOnGround = false;
	virtual void Render();
};
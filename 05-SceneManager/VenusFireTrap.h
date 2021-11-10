#pragma once
#include "GameObject.h"
#include "Game.h"
#include"Animation.h"
#include"Animations.h"
#include "Timer.h"

#define VENUS_MOVING_SPEED 0.005f

#define HIDDEN_TIME 2500
#define ATTACK_TIME 2500
#define VENUS_BBOX_WIDTH 16
#define VENUS_BBOX_HEIGHT 32

#define VENUS_STATE_UP 100
#define VENUS_STATE_DOWN 200
#define VENUS_STATE_FIRE 300
#define VENUS_STATE_HIDDEN 400
#define VENUS_STATE_ATTACK 500

#define ID_ANI_REDVENUS_RIGHT_DOWN 6000
#define ID_ANI_REDVENUS_RIGHT_UP 6001
#define ID_ANI_REDVENUS_LEFT_DOWN 6002
#define ID_ANI_REDVENUS_LEFT_UP 6003
#define ID_ANI_REDVENUS_MOVE 6004

#define ID_ANI_SET_REDVENUS 1
class CVenusFireTrap : public CGameObject
{
protected:
	float ax;
	float ay;
	Timer* hiddenTimer;
	Timer* attackTimer;
	ULONGLONG die_start;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CVenusFireTrap(float x, float y);
	virtual void SetState(int state);
};
#pragma once
#include "GameObject.h"
#include "Timer.h"
#include "Ghost.h"
#define TROOPA_GRAVITY 0.002f
#define TROOPA_WALKING_SPEED 0.05f
#define TROOPA_ROLLING_SPEED 0.1f
#define TROOPA_JUMP_SPEED 0.4f

#define TROOPA_BBOX_WIDTH 16
#define TROOPA_BBOX_HEIGHT 26
#define TROOPA_BBOX_HEIGHT_DIE 16



#define TROOPA_STATE_WALKING 100
//#define TROOPA_STATE_ROLL 300
#define TROOPA_STATE_DIE 200
#define TROOPA_STATE_ROLL_LEFT 400
#define TROOPA_STATE_ROLL_RIGHT 600
#define TROOPA_STATE_JUMP 500
#define TROOPA_STATE_DIE_UP	700

#define KOOPAS_TYPE_RED 1000
#define KOOPAS_TYPE_GREEN 2000
#define KOOPAS_TYPE_GREEN_WING 3000

#define ID_ANI_RED_TROOPA_WALKING_LEFT 6101
#define ID_ANI_RED_TROOPA_DIE_UP_IDLE 6103
#define ID_ANI_RED_TROOPA_DIE_DOWN_IDLE 6102
#define ID_ANI_RED_TROOPA_WALKING_RIGHT 6100
#define ID_ANI_RED_TROOPA_DIE_UP_RUN 6105
#define ID_ANI_RED_TROOPA_DIE_DOWN_RUN 6104
#define ID_ANI_RED_TROOPA_DIE_DOWN_SHAKE 6106

#define ID_ANI_GREEN_TROOPA_FLY_LEFT 6202
#define ID_ANI_GREEN_TROOPA_FLY_RIGHT 6201
#define ID_ANI_GREEN_TROOPA_WALKING_LEFT 6204
#define ID_ANI_GREEN_TROOPA_DIE_UP_IDLE 6206
#define ID_ANI_GREEN_TROOPA_DIE_DOWN_IDLE 6205
#define ID_ANI_GREEN_TROOPA_WALKING_RIGHT 6203
#define ID_ANI_GREEN_TROOPA_DIE_UP_RUN 6208
#define ID_ANI_GREEN_TROOPA_DIE_DOWN_RUN 6207
#define ID_ANI_GREEN_TROOPA_DIE_DOWN_SHAKE 6209

#define KOOPA_LEVEL_WING 2

class CKoopas : public CGameObject
{
protected:
	float ax;
	float ay;
	
	float start_vx;
	int level;
	float koopa_type;
	
	ULONGLONG die_start;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlockingX() { return 1; }
	virtual int IsBlockingY() { return 1; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithBrick(LPCOLLISIONEVENT e);
public:
	CGhost* ghost = NULL;
	bool isGhostFollow = false;
	CKoopas(float x, float y,float type);
	int GetLevel() { return level; };
	void SetLevel(int level) { this->level = level; };
	int GetType() { return type; };
	void SetType(int type) { this->type = type; };
	int GetKoopasType() { return koopa_type; };
	void SetKoopasType(int koopa_type) { this->koopa_type = koopa_type; };
	virtual void SetState(int state);
	Timer* timeStartJump = new Timer(2000);
	Timer* timeReborn = new Timer(5000);
	bool isOnGround = false;
	BOOLEAN isMariohold = false;
	BOOLEAN isShellUp = false;
	int n; //0: left, 1:right
	float temp_x;
	float temp_y;
};
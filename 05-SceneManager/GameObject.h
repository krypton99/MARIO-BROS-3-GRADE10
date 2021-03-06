#pragma once

#include <Windows.h>
#include <d3dx10.h>
#include <vector>

#include "Animation.h"
#include "Animations.h"
#include "Sprites.h"
#include "Collision.h"
#include "AnimationSet.h"

using namespace std;

#define ID_TEX_BBOX -100		// special texture to draw object bounding box
#define BBOX_ALPHA 0.25f		// Bounding box transparency
#define STATE_ERASE 10
class CGameObject
{
protected:

	float x;
	float y;
	float start_x;
	float start_y;
	float vx;
	float vy;
	float type;
	int nx;

	int state;

	bool isDeleted;
	LPANIMATION_SET animation_set;
public:
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }
	void GetPosition(float& x, float& y) { x = this->x; y = this->y; }
	void GetSpeed(float& vx, float& vy) { vx = this->vx; vy = this->vy; }
	int GetType() { return this->type; }
	void SetType(int type) { this->type = type; }
	void SetAnimationSet(LPANIMATION_SET ani_set) { animation_set = ani_set; }
	int GetState() { return this->state; }
	virtual void Delete() { isDeleted = true; }
	bool IsDeleted() { return isDeleted; }
	bool isInGrid;
	void RenderBoundingBox();

	CGameObject();
	CGameObject(float x, float y) :CGameObject() { this->x = x; this->y = y; }
	//CGameObject(float x, float y, float type) :CGameObject() { this->x = x; this->y = y; this->type = type; }
	
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) {};
	virtual void Render() = 0;
	virtual void SetState(int state) { this->state = state; }

	//
	// Collision ON or OFF ? This can change depending on object's state. For example: die
	//
	virtual int IsCollidable() { return 0; };
	virtual int IsCollidableX() { return 0; };
	virtual int IsCollidableY() { return 0; };
	// When no collision has been detected (triggered by CCollision::Process)
	virtual void OnNoCollision(DWORD dt) {};

	// When collision with an object has been detected (triggered by CCollision::Process)
	virtual void OnCollisionWith(LPCOLLISIONEVENT e) {};

	// Is this object blocking other object? If YES, collision framework will automatically push the other object
	virtual int IsBlockingX() { return 1; }
	virtual int IsBlockingY() { return 1; }

	~CGameObject();

	static bool IsDeleted(const LPGAMEOBJECT& o) { return o->isDeleted; }
};
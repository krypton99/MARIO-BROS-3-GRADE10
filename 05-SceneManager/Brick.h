#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_BRICK 10000
#define ID_ANI_QUESTION_BRICK_ACTIVE 11001
#define ID_ANI_QUESTION_BRICK_EMPTY 11002

#define BRICK_TYPE_QUESTION 100
#define BRICK_TYPE_HIDDEN 200

#define BRICK_STATE_ACTIVE 1
#define BRICK_STATE_EMPTY 0
#define BRICK_STATE_BOUND 2

#define BRICK_WIDTH 16
#define BRICK_BBOX_WIDTH 16
#define BRICK_BBOX_HEIGHT 16

#define CONTAIN_NONE			0
#define CONTAIN_MUSHROOM		1 
#define CONTAIN_LEAF	2

class CBrick : public CGameObject {
private:
	float brickType;
	float itemType;
	
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	virtual void OnNoCollision(DWORD dt);
public:
	
	bool isFallingItem = false;
	CBrick(float x, float y, float type, float itemType);
	void Render();
	int GetBrickType() { return brickType; };
	void SetBrickType(int brickType) { this->brickType = brickType; }
	int GetItemType() { return itemType; };
	void SetItemType(int itemType) { this->itemType = itemType; };
	float GetPosX() { return x; };
	float GetPosY() { return y; };
	virtual int IsCollidable() { return 1; };
	virtual int IsBlockingX() { return isBlocking; };
	virtual int IsBlockingY() { return isBlocking; };
	virtual void SetState(int state);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	boolean isBlocking=1;
};
#pragma once
#include "Item.h"
#include "GameObject.h"

#define LEAF_SPEED_FALL	0.03f
#define LEAF_SPEED_X	0.08f
class CLeaf : public Item
{
private:
	float ax;
	float ay;
	virtual int IsCollidable() { return 1; };
	virtual int IsBlockingX() { return 0; };
	virtual int IsBlockingY() { return 0; };
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnCollisionWithMario(LPCOLLISIONEVENT e);
public:

	CLeaf(float x, float y, float leafType);
	~CLeaf() {}
	BOOLEAN isFalling = false;
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
};

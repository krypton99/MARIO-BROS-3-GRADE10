#pragma once
#include "Item.h"
#include "GameObject.h"
class CMushroom : public Item
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

	CMushroom(float x,float y, float mushroomType);
	~CMushroom() {}

	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
};

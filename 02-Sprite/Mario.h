#pragma once

#include "GameObject.h"

#include "Animation.h"
#include "Animations.h"

#define MARIO_WIDTH 14
#define GOOMBA_WIDTH 20

#define ID_PLATFORM_LEFT 1
#define ID_PLATFORM_RIGHT 2
#define ID_PLATFORM_MIDDLE 3
#define ID_TREE_1 4
#define ID_TREE_2 5
#define ID_TREE_3 6
#define ID_TREE_4 7
#define ID_TREE_5 8
#define ID_TREE_6 9
#define ID_TREE_7 10
#define ID_BUSH 11
#define ID_BRICK_QUESTION 12

//#define ID_PLATFORM_RIGTH 2

class CBrick : public CGameObject {
public: 
	CBrick(float x, float y) : CGameObject(x, y) {}
	void Render();
	void Update(DWORD dt) {}
};

class CTile : public CGameObject {
	LPANIMATION animation_set;
	int tileid;
public:
	CTile(float x, float y) : CGameObject(x, y) {};
	void setTileid(int id) { tileid = id; }
	int getTileid() { return tileid; }
	void Render();
	void SetAnimationSet(LPANIMATION ani_set) { animation_set = ani_set; }
	void Update(DWORD dt) {}
};

class CMario : public CGameObject
{
protected:
	float vx;
public:
	CMario(float x, float y, float vx);
	void Update(DWORD dt);
	void Render();
};

class CGoomba : public CGameObject
{
protected:
	float vx;
public:
	CGoomba(float x, float y, float vx);
	void Update(DWORD dt);
	void Render();
};



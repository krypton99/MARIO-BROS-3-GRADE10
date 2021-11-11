#pragma once
#include "GameObject.h"
#define FLAT_BBOX 16

class CFlat : public CGameObject
{
	int width;
	int height;
public:
	bool isInteract; // isInteract -> 1: color box -> cho phep di xuyen

	CFlat(int width, int height, bool isInteract);
	~CFlat();

	int GetWidth() { return this->width; }
	int GetHeight() { return this->height; }

	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};
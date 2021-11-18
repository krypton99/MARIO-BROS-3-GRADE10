#pragma once

#include "GameObject.h"
#include "AssetIDs.h"
// 
// The most popular type of object in Mario! 
// 
class CPlatform : public CGameObject
{
protected:
	int length;				// Unit: cell 
	float cellWidth;
	float cellHeight;
	int spriteIdBegin, spriteIdMiddle, spriteIdEnd;
	
public:
	CPlatform(float x, float y,
		float cell_width, float cell_height, int length,
		int sprite_id_begin, int sprite_id_middle, int sprite_id_end, bool isThrough) :CGameObject(x, y)
	{
		this->length = length;
		this->cellWidth = cell_width;
		this->cellHeight = cell_height;
		this->spriteIdBegin = sprite_id_begin;
		this->spriteIdMiddle = sprite_id_middle;
		this->spriteIdEnd = sprite_id_end;
		this->isThrough = isThrough;
		type = OBJECT_TYPE_PLATFORM;
	}
	/*int IsCollidableX()
	{
		return collideX;
	}
	int IsCollidableY()
	{
		return collideY;
	}
	int IsCollidable()
	{
		return (!collideX && !collideY);
	}*/
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual int IsBlockingX() { return !isThrough; };
	virtual int IsBlockingY() { return isBlocking; };
	bool isThrough;
	
	void Render();
	//void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void RenderBoundingBox();
	//boolean isThrough = 0;
	boolean collideX = 0;
	boolean collideY = 0;
	boolean isBlocking = 1;
	boolean isOnY = 0;
};

typedef CPlatform* LPPLATFORM;
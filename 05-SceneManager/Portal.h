#pragma once

#include "GameObject.h"

/*
	Object that triggers scene switching
*/
class CPortal : public CGameObject
{
	int scene_id;	// target scene to switch to 
	int portal_in_type; //0 down, 1 up
	int portal_out_type; //0 down, 1 up
	float width;
	float height;
	
public:
	CPortal(float l, float t, float r, float b, int scene_id, int portal_in_type,float x, float y, int portal_out_type);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);

	void RenderBoundingBox(void);
	int GetPortalInType() {
		return portal_in_type;
	}
	int GetPortalOutType() {
		return portal_out_type;
	}
	int GetSceneId() { return scene_id; }
	virtual int IsBlockingX() { return 1; };
	virtual int IsBlockingY() { return 1; };
	virtual int IsCollidable() { return 1; };
	float player_x;
	float player_y;
};

#include "Item.h"

Item::Item() {
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(30));
}


void Item::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	CGameObject::Update(dt);
}

void Item::GetBoundingBox(float& l, float& t, float& r, float& b) {
	l = x - ITEM_BBOX / 2;
	t = y - ITEM_BBOX / 2;
	r = l + ITEM_BBOX;
	b = t + ITEM_BBOX;
}
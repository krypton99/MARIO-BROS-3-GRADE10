#include "Flat.h"

CFlat::CFlat(int width, int height, bool isInteract) {

	this->width = width;//so luong vien
	this->height = height;//so luong vien
	this->isInteract = isInteract;
}

CFlat::~CFlat() {}

void CFlat::Render() {
	//animation_set->at(0)->Render(x, y);
	//RenderBoundingBox();
}

void CFlat::GetBoundingBox(float& l, float& t, float& r, float& b) {
	l = x;
	t = y;
	r = l + FLAT_BBOX * width;
	b = t + FLAT_BBOX * height;
}
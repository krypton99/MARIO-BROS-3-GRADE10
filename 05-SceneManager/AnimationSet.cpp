#include "AnimationSet.h"
#include "Utils.h"

CAnimationSets* CAnimationSets::__instance = NULL;

CAnimationSets::CAnimationSets() {

}

CAnimationSets* CAnimationSets::GetInstance()
{
	if (__instance == NULL) __instance = new CAnimationSets();
	return __instance;
}

LPANIMATION_SET CAnimationSets::Get(unsigned int id)
{
	LPANIMATION_SET ani_set = animation_sets[id];
	if (ani_set == NULL)


		return ani_set;
	else return NULL;
}

void CAnimationSets::Add(int id, LPANIMATION_SET ani_set)
{
	animation_sets[id] = ani_set;
}
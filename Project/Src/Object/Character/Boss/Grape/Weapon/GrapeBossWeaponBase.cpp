#include "GrapeBossWeaponBase.h"


GrapeBossWeaponBase::GrapeBossWeaponBase(int model)
	: ActorBase("Data/Parameter/Character/Boss/Grape/Weapons/"), collBack(0), collFront(0)
{
	trans.Duplicate(model);
}

void GrapeBossWeaponBase::SubDraw(void)
{
	trans.Draw();

	SetUseLighting(false);
	collFront.Draw();
	collBack.Draw();
	SetUseLighting(true);
}

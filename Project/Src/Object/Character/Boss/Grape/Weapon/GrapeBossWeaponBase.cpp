#include "GrapeBossWeaponBase.h"


GrapeBossWeaponBase::GrapeBossWeaponBase(std::string parameterPath, int model)
	: collBack(0), collFront(0)
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

#include "GrapeBossWeaponBase.h"


GrapeBossWeaponBase::GrapeBossWeaponBase(int model)
	: ActorBase("Data/Parameter/Character/Boss/Grape/Weapons/"), collBack(0), collFront(0)
{
	trans.Duplicate(model);

	collBack.Load("Range/CircleRangeBack");
	collFront.Load("Range/CircleRangeFront");

}

void GrapeBossWeaponBase::SubDraw(void)
{
	trans.Draw();

	SetUseLighting(false);
	collBack.Draw();
	collFront.Draw();
	SetUseLighting(true);
}

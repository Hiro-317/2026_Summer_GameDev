#include "GrapeBossWeaponBase.h"

GrapeBossWeaponBase::GrapeBossWeaponBase(int model, int front, int back) :
	ActorBase("Data/Parameter/Character/Boss/Grape/Weapons/"),

	collBack(),
	collFront(),

	state(STATE::None),

	count()
{
	trans.model = model;
	collFront.model = front;
	collBack.model = back;
}

void GrapeBossWeaponBase::SubDraw(void)
{
	SetUseLighting(false);
	collBack.Draw();
	collFront.Draw();
	SetUseLighting(true);
}

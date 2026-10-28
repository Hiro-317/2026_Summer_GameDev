#include "GrapeBossWeaponBase.h"

#include "../../../../Common/Collider/ColliderBase.h"

GrapeBossWeaponBase::GrapeBossWeaponBase(int model, int front, int back)
	: ActorBase("Data/Parameter/Character/Boss/Grape/Weapons/"), collBack(), collFront()
{
	trans.Duplicate(model);
	collFront.model = front;
	collBack.model = back;
}

void GrapeBossWeaponBase::SetColliderFlg(bool flg)
{
	ColliderSerch(GetColliders(), COLLIDER_TAG::BossAttackArea).back()->SetJudgeFlg(flg);
	SetIsDraw(flg);
}

void GrapeBossWeaponBase::SubDraw(void)
{
	SetUseLighting(false);
	collBack.Draw();
	collFront.Draw();
	SetUseLighting(true);
}

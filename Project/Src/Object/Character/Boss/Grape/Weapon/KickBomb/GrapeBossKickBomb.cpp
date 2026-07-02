#include "GrapeBossKickBomb.h"

#include "../../../../../Common/Collider/SphereCollider.h"


GrapeBossKickBomb::GrapeBossKickBomb(int model)
	: GrapeBossWeaponBase("path", model)
{
	// 当たり判定を生成する（球体コライダー）
	ColliderCreate(
		new SphereCollider(
			COLLIDER_TAG::BOSS,
			GetParameter("KickBomb", "Radius")
		)
	);
}

void GrapeBossKickBomb::SubUpdate(void)
{
}

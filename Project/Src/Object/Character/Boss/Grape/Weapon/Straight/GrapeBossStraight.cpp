#include "GrapeBossStraight.h"

#include "../../../../../Common/Collider/SphereCollider.h"

GrapeBossStraight::GrapeBossStraight(int model)
	: GrapeBossWeaponBase("param", model)
{
	trans.Duplicate(model);

	// 当たり判定を生成する（球体コライダー）
	ColliderCreate(
		new SphereCollider(
			COLLIDER_TAG::BOSS,
			GetParameter("Straight", "Radius")
			)
	);
}

void GrapeBossStraight::SubUpdate(void)
{
	trans.pos += moveDir * MOVE_SPEED;
}

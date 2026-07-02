#include "GrapeBossStraight.h"

#include "../../../../../Common/Collider/SphereCollider.h"


GrapeBossStraight::GrapeBossStraight(int model)
	: GrapeBossWeaponBase("param", model)
{
	// 当たり判定を生成する（球体コライダー）
	ColliderCreate(
		new SphereCollider(
			COLLIDER_TAG::BOSS,
			GetParameter("Straight", "Radius")
			)
	);

	// 重力無視
	SetGravityFlg(false);
}

void GrapeBossStraight::Load(const MSG_SENDER_ID operatorSenderId, const CharacterStats& stats)
{
	CreateAttackSkill(operatorSenderId, 50, &stats);

	collBack.scale = DEFAULT_VIEW_SCALE;
	collFront.scale = DEFAULT_VIEW_SCALE - Vector3::Xonly(DEFAULT_VIEW_SCALE.x);
}

void GrapeBossStraight::SubUpdate(void)
{
	trans.pos += moveDir * MOVE_SPEED;
}

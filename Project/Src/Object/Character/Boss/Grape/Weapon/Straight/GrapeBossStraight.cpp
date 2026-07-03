#include "GrapeBossStraight.h"

#include "../../../../../Common/Collider/SphereCollider.h"


GrapeBossStraight::GrapeBossStraight(int model)
	: GrapeBossWeaponBase("param", model)
{
	// 当たり判定を生成する（球体コライダー）
	ColliderCreate(
		new SphereCollider(
			COLLIDER_TAG::BOSS_ATTACK,
			GetParameter("Straight", "Radius")
			)
	);

	// 重力無視
	SetGravityFlg(false);
}

void GrapeBossStraight::Load(const MSG_SENDER_ID operatorSenderId, const CharacterStats& stats)
{
	CreateAttackSkill(operatorSenderId, 50, &stats);

	collBack.scale = ATTACK_RANGE;
	collFront.scale = ATTACK_RANGE - Vector3::Xonly(ATTACK_RANGE.x);
}

void GrapeBossStraight::SubUpdate(void)
{
	trans.pos += moveDir * MOVE_SPEED;
}

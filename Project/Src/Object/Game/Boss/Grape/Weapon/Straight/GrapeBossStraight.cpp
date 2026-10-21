#include "GrapeBossStraight.h"

#include "../../../../../Common/Collider/SphereCollider.h"


GrapeBossStraight::GrapeBossStraight(int model, int front, int back)
	: GrapeBossWeaponBase(model, front, back)
{
	// 当たり判定を生成する（球体コライダー）
	AddCollider(
		new SphereCollider(
			COLLIDER_TAG::BossAttack,
			GetParameter("Straight", "Radius") * ATTACK_SIZE
			)
	);
	AddCollider(
		new SphereCollider(
			COLLIDER_TAG::BossAttackArea,
			GetParameter("Straight", "AreaRadius") * ATTACK_SIZE
			)
	);

	trans.scale = Vector3(ATTACK_SIZE);

	SetJudgeFlg(false);
	SetPushFlg(false);
	// 重力無視
	SetGravityFlg(false);
}

void GrapeBossStraight::Load(const MSG_SENDER_ID controlSenderId, const CharacterStats& stats)
{
	CreateAttackSkill(controlSenderId, 50, &stats);

	collBack.scale = Vector3::XZonly(ATTACK_SIZE, ATTACK_SIZE);
	collFront.scale = Vector3::Zonly(ATTACK_SIZE);
}

void GrapeBossStraight::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const Vector3& collisionPoint)
{
	if (ownTag == COLLIDER_TAG::BossAttackArea) {

		end = true;
		SetJudgeFlg(false);
	}
}

void GrapeBossStraight::SubUpdate(void)
{
	// 進行方向に動かす
	trans.pos += moveDir * MOVE_SPEED;
}

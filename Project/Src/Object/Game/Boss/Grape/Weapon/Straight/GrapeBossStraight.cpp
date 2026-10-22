#include "GrapeBossStraight.h"

#include "../../../../../Common/Collider/SphereCollider.h"

#include "../../../../../Common/CharacterBase/CharacterStatsDefine.h"

GrapeBossStraight::GrapeBossStraight(int model, int front, int back) :
	GrapeBossWeaponBase(model, front, back)
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
			(GetParameter("Straight", "Radius") * ATTACK_SIZE) * 0.9f
		)
	);

	trans.scale = ATTACK_SIZE;

	SetJudgeFlg(false);
	SetPushFlg(false);
	// 重力無視
	SetGravityFlg(false);
}

void GrapeBossStraight::Load(const MSG_SENDER_ID controlSenderId, const CharacterStats& stats)
{
	GetColliders().back()->SetSkillStats(new SkillStats(50, &stats));

	collBack.scale = Vector3::XZonly(ATTACK_SIZE, ATTACK_SIZE);
	collFront.scale = Vector3::Zonly(ATTACK_SIZE);
}

void GrapeBossStraight::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)
{
	if (other.GetTag() == COLLIDER_TAG::Stage) {
		state = STATE::None;
		SetJudgeFlg(false);
	}
}

void GrapeBossStraight::SubUpdate(void)
{
	// 進行方向に動かす
	trans.pos += moveVec;
}

void GrapeBossStraight::Shot(const Vector3& pos, const Vector3& vec)
{
	trans.pos = pos;
	moveVec = vec * MOVE_SPEED;

	SetJudgeFlg(true);

	SetIsDraw(true);
}
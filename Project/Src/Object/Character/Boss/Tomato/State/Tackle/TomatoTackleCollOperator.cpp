#include "TomatoTackleCollOperator.h"

#include "../../../../../Common/Collider/XZCircleCollider.h"

TomatoTackleCollOperator::TomatoTackleCollOperator(
	const CharacterStats stats,
	const float TO_PLAYER_DISTANCE,
	const Vector3& pos
)
	: stats(stats),
	TO_PLAYER_DISTANCE(TO_PLAYER_DISTANCE + 5.0f),
	pos(pos)
{
	trans.pos = pos;
}

void TomatoTackleCollOperator::Load(void)
{
	// 当たり判定を生成する（XZコライダー）
	ColliderCreate(
		new XZCircleCollider(COLLIDER_TAG::BOSS_ATTACK, TO_PLAYER_DISTANCE)
	);
	// ステージ当たり判定を生成する（XZコライダー）
	ColliderCreate(
		new XZCircleCollider(COLLIDER_TAG::BOSS_ATTACK_AREA, TO_PLAYER_DISTANCE)
	);

	CreateAttackSkill(150, &stats, COLLIDER_TAG::BOSS_ATTACK);

	SetJudge(true);
	ColliderSerch(COLLIDER_TAG::BOSS_ATTACK).back()->SetJudgeFlg(false);
}

void TomatoTackleCollOperator::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other)
{
	if (other.GetTag() == COLLIDER_TAG::STAGE && other.GetShape() == ColliderBase::SHAPE::XZ_CIRCLE) {

		stageHit = true;
	}
}

void TomatoTackleCollOperator::SubUpdate(void)
{
	trans.pos = pos;
}

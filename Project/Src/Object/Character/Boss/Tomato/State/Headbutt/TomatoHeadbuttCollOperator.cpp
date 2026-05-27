#include "TomatoHeadbuttCollOperator.h"

#include "../../../../../Common/Collider/XZCircleCollider.h"

TomatoHeadbuttCollOperator::TomatoHeadbuttCollOperator(
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

void TomatoHeadbuttCollOperator::Load(void)
{
	// プレイヤー当たり判定を生成する（XZコライダー）
	ColliderCreate(
		new XZCircleCollider(COLLIDER_TAG::BOSS_ATTACK, TO_PLAYER_DISTANCE)
	);

	CreateAttackSkill(75, &stats, COLLIDER_TAG::BOSS_ATTACK);

	SetJudge(false);
}

void TomatoHeadbuttCollOperator::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other)
{
}

void TomatoHeadbuttCollOperator::SubUpdate(void)
{
	trans.pos = pos;
}

void TomatoHeadbuttCollOperator::SubDraw(void) {
}
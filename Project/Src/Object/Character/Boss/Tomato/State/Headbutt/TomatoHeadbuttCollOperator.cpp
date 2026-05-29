#include "TomatoHeadbuttCollOperator.h"

#include "../../../../../Common/Collider/XZCircleCollider.h"

TomatoHeadbuttCollOperator::TomatoHeadbuttCollOperator(
	const float TO_PLAYER_DISTANCE,
	const CharacterStats stats,
	const ParameterLoad& collParam
)
	: stats(stats),
	TO_PLAYER_DISTANCE(TO_PLAYER_DISTANCE + 5.0f),
	collParam(collParam)
{
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

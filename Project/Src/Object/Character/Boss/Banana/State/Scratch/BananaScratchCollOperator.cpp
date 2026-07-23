#include "BananaScratchCollOperator.h"

#include "../../../../../Common/Collider/CapsuleCollider.h"

BananaScratchCollOperator::BananaScratchCollOperator(
	const float TO_PLAYER_DISTANCE,
	const MSG_SENDER_ID operatorSenderId,
	const CharacterStats& stats,
	const ParameterLoad& collParam,
	const std::function<Vector3(void)> StartPos,
	const std::function<Vector3(void)> EndPos
) :
	operatorSenderId(operatorSenderId),
	stats(stats),
	TO_PLAYER_DISTANCE(TO_PLAYER_DISTANCE + 5.0f),
	collBack(),
	collFront()
{
}

void BananaScratchCollOperator::Load(void)
{
	// プレイヤー当たり判定を生成する（XZコライダー）
	ColliderCreate(
		new CapsuleCollider(COLLIDER_TAG::BOSS_ATTACK, StartPos(), EndPos(), 30.0f);
	);

	CreateAttackSkill(operatorSenderId, 75, &stats, COLLIDER_TAG::BOSS_ATTACK);

	SetJudge(false);

	collBack.Load("Range/LineRangeBack");
	collFront.Load("Range/LineRangeFront");

	collBack.pos = Vector3::Yonly(HEIGHT);
	collFront.pos = Vector3::Yonly(HEIGHT);

	collBack.scale = SCALE;
	collFront.scale = Vector3(SCALE.x, SCALE.y, 0.0f);

	isDrawArea = false;
}

void BananaScratchCollOperator::SubAlphaDraw(void)
{
	if (isDrawArea) {
		SetUseLighting(false);
		collFront.Draw();
		collBack.Draw();
		SetUseLighting(true);
	}
}

#include "GrapeStampCollOperator.h"

#include "../../../../../Common/Collider/XZCircleCollider.h"

#include "../../../../../Common/CharacterBase/CharacterStatsDefine.h"

GrapeStampCollOperator::GrapeStampCollOperator(
	const MSG_SENDER_ID operatorSenderId,
	const CharacterStats& stats
)	:
	operatorSenderId(operatorSenderId),
	stats(stats),
	collBack(),
	collFront()
{
}

void GrapeStampCollOperator::Load(void)
{
	// プレイヤー当たり判定を生成する（XZコライダー）
	AddCollider(
		new XZCircleCollider(COLLIDER_TAG::BossAttack, RADIUS, 1000.0f)
	);
	// ステージ当たり判定を生成する（XZコライダー）
	AddCollider(
		new XZCircleCollider(COLLIDER_TAG::BossAttackArea, RADIUS, 1000.0f)
	);

	ColliderSerch(GetColliders(), COLLIDER_TAG::BossAttack).back()->SetSkillStats(new SkillStats(75, &stats));

	SetJudgeFlg(false);
	ColliderSerch(GetColliders(), COLLIDER_TAG::BossAttackArea).back()->SetJudgeFlg(true);
	ColliderSerch(GetColliders(), COLLIDER_TAG::BossAttackArea).back()->SetPushFlg(true);

	collBack.LoadModel("Range/CircleRangeBack");
	collFront.LoadModel("Range/CircleRangeFront");

	collBack.pos = Vector3::Yonly(HEIGHT);
	collFront.pos = Vector3::Yonly(HEIGHT + 1.0f);

	collBack.scale = SCALE;
	collFront.scale = 0.0f;

	isDrawArea = false;
}

void GrapeStampCollOperator::SubUpdate(void)
{
	collBack.pos = Vector3(trans.pos.x, HEIGHT, trans.pos.z);
	collFront.pos = Vector3(trans.pos.x, HEIGHT, trans.pos.z);
}

void GrapeStampCollOperator::SubDraw(void)
{
	if (isDrawArea) {
		SetUseLighting(false);
		collBack.Draw();
		collFront.Draw();
		SetUseLighting(true);
	}
}

void GrapeStampCollOperator::SubRelease(void)
{
	collBack.Release();
	collFront.Release();
}

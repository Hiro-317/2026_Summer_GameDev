#include "GrapeStampCollOperator.h"

#include "../../../../../Common/Collider/XZCircleCollider.h"

GrapeStampCollOperator::GrapeStampCollOperator(
	const MSG_SENDER_ID controlSenderId,
	const CharacterStats& stats
)	:
	controlSenderId(controlSenderId),
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

	CreateAttackSkill(controlSenderId, 75, &stats, COLLIDER_TAG::BossAttack);

	SetJudgeFlg(false);
	ColliderSerch(GetCollider(), COLLIDER_TAG::BossAttackArea).back()->SetJudgeFlg(true);
	ColliderSerch(GetCollider(), COLLIDER_TAG::BossAttackArea).back()->SetDynamicFlg(true);
	ColliderSerch(GetCollider(), COLLIDER_TAG::BossAttackArea).back()->SetPushFlg(true);

	collBack.Load("Range/CircleRangeBack");
	collFront.Load("Range/CircleRangeFront");

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

void GrapeStampCollOperator::SubAlphaDraw(void)
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

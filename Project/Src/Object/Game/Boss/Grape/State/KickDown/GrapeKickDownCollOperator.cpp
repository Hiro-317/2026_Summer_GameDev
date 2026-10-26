#include "GrapeKickDownCollOperator.h"

#include "../../../../../Common/Collider/XZCircleCollider.h"

#include "../../../../../Common/CharacterBase/CharacterStatsDefine.h"

GrapeKickDownCollOperator::GrapeKickDownCollOperator(
	const MSG_SENDER_ID& controlSenderId,
	const CharacterStats& stats
)	:
	controlSenderId(controlSenderId),
	stats(stats),
	collBack(),
	collFront()
{
}

void GrapeKickDownCollOperator::Load(void)
{
	// プレイヤー当たり判定を生成する（XZコライダー）
	AddCollider(new XZCircleCollider(COLLIDER_TAG::BossAttack, ATTACK_RADIUS, 1000.0f));

	GetColliders().back()->SetSkillStats(new SkillStats(75, &stats));

	SetJudgeFlg(false);

	collBack.LoadModel("Range/CircleRangeBack");
	collFront.LoadModel("Range/CircleRangeFront");

	collBack.pos = Vector3::Yonly(HEIGHT);
	collFront.pos = Vector3::Yonly(HEIGHT + 1.0f);

	collBack.scale = SCALE;
	collFront.scale = 0.0f;

	isDrawArea = false;
}

void GrapeKickDownCollOperator::SubUpdate(void)
{
	if (GetJudgeFlg()) {

	}

	if (isDrawArea) {
		collBack.pos = Vector3(trans.pos.x, HEIGHT, trans.pos.z);
		collFront.pos = Vector3(trans.pos.x, HEIGHT, trans.pos.z);
	}
}

void GrapeKickDownCollOperator::SubDraw(void)
{
	if (isDrawArea) {
		SetUseLighting(false);
		collBack.Draw();
		collFront.Draw();
		SetUseLighting(true);
	}
}

void GrapeKickDownCollOperator::SubRelease(void)
{
	collBack.Release();
	collFront.Release();
}

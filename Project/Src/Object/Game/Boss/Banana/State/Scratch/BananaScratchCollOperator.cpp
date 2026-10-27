#include "BananaScratchCollOperator.h"

#include "../../../../../Common/CharacterBase/CharacterStatsDefine.h"

BananaScratchCollOperator::BananaScratchCollOperator(
	const MSG_SENDER_ID controlSenderId,
	const CharacterStats& stats,
	const Vector3& pos,
	const Vector3& StartPos,
	const Vector3& EndPos
) :
	controlSenderId(controlSenderId),
	stats(stats),
	pos(pos),
	StartPos(StartPos), EndPos(EndPos),
	collBack(),	collFront()
{
}

void BananaScratchCollOperator::Load(void)
{
	// プレイヤー当たり判定を生成する（XZコライダー）
	collRef = new CapsuleCollider(COLLIDER_TAG::BossAttack, StartPos, EndPos, RADIUS);
	AddCollider(collRef);

	collRef->SetSkillStats(new SkillStats(75, &stats));
	SetPushFlg(false);
	SetJudgeFlg(false);

	collBack.centerDiff = Vector3::XZonly(pos.x, pos.z) + DIFF;
	collFront.centerDiff = Vector3::XZonly(pos.x, pos.z) + DIFF;

	collBack.LoadModel("Range/CircleRangeBack");
	collFront.LoadModel("Range/CircleRangeFront");

	collBack.pos = Vector3::Yonly(HEIGHT);
	collFront.pos = Vector3::Yonly(HEIGHT);

	collBack.scale = SCALE;
	collFront.scale = Vector3(0.0f);

	SetDrawType(ACTOR_DRAW_TYPE::Alpha);
	isDrawArea = false;
}

void BananaScratchCollOperator::SubDraw(void)
{
	if (isDrawArea) {
		SetUseLighting(false);
		collBack.Draw();
		collFront.Draw();
		SetUseLighting(true);
	}
}

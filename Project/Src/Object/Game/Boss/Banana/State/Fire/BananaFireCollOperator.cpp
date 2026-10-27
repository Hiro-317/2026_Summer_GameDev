#include "BananaFireCollOperator.h"

#include "../../../../../Common/CharacterBase/CharacterStatsDefine.h"


BananaFireCollOperator::BananaFireCollOperator(
	const MSG_SENDER_ID controlSenderId,
	const CharacterStats& stats
) :
	controlSenderId(controlSenderId),
	stats(stats)
{
}

void BananaFireCollOperator::Load(void)
{
	// プレイヤー当たり判定を生成する（球体コライダー）
	collRef = new SphereCollider(COLLIDER_TAG::BossAttack, RADIUS);
	AddCollider(collRef);

	collRef->SetSkillStats(new SkillStats(100, &stats));
	SetPushFlg(false);
	SetJudgeFlg(false);

	ColliderSerch(GetColliders(), COLLIDER_TAG::BossAttack).back()->SetTransformPos(POS);

	collBack.LoadModel("Range/CircleRangeBack");
	collFront.LoadModel("Range/CircleRangeFront");

	collBack.pos = POS;
	collFront.pos = POS;

	collBack.scale = SCALE;
	collFront.scale = Vector3(0.0f);

	isDrawArea = false;
	SetDrawType(ACTOR_DRAW_TYPE::Alpha);
}

void BananaFireCollOperator::SubDraw(void)
{
	if (isDrawArea) {
		SetUseLighting(false);
		collBack.Draw();
		collFront.Draw();
		SetUseLighting(true);
	}
}

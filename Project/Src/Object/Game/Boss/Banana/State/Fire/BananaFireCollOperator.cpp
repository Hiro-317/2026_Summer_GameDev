#include "BananaFireCollOperator.h"


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
	AddCollider(
		new SphereCollider(COLLIDER_TAG::BossAttack, RADIUS)
	);

	CreateAttackSkill(controlSenderId, 100, &stats, COLLIDER_TAG::BossAttack);
	SetPushFlg(false);
	SetJudgeFlg(false);

	ColliderSerch(GetCollider(), COLLIDER_TAG::BossAttack).back()->SetTransformPos(POS);

	collBack.Load("Range/CircleRangeBack");
	collFront.Load("Range/CircleRangeFront");

	collBack.pos = POS;
	collFront.pos = POS;

	collBack.scale = SCALE;
	collFront.scale = Vector3(0.0f);

	isDrawArea = false;
}

void BananaFireCollOperator::SubAlphaDraw(void)
{
	if (isDrawArea) {
		SetUseLighting(false);
		collBack.Draw();
		collFront.Draw();
		SetUseLighting(true);
	}
}

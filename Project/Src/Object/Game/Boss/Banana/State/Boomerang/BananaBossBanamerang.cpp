#include "BananaBossBanamerang.h"


BananaBossBanamerang::BananaBossBanamerang(
	const MSG_SENDER_ID controlSenderId,
	const CharacterStats& stats
) :
	controlSenderId(controlSenderId),
	stats(stats)
{
	banamerang.Load("Character/Banana/Banamerang");
	banamerang.scale = SIZE;
}

void BananaBossBanamerang::Load(void)
{
	// プレイヤー当たり判定を生成する（XZコライダー）
	AddCollider(
		new XZCircleCollider(COLLIDER_TAG::BossAttack, RADIUS, 120.0f)
	);

	CreateAttackSkill(controlSenderId, 85, &stats, COLLIDER_TAG::BossAttack);
	SetPushFlg(false);
	SetJudgeFlg(false);

	ColliderSerch(GetCollider(), COLLIDER_TAG::BossAttack).back()->SetTransformPtr(&banamerang);

	collBack.Load("Range/CircleHalfRangeBack");
	collFront.Load("Range/CircleHalfRangeFront");

	collBack.pos = POS;
	collFront.pos = POS;

	collBack.scale = SCALE;
	collFront.scale = Vector3(0.0f);

	collBack.localRotation = Quaternion::FromRotationY(Deg2Rad(180.0f));
	collFront.localRotation = Quaternion::FromRotationY(Deg2Rad(180.0f));

	isDrawArea = false;
}

void BananaBossBanamerang::SubAlphaDraw(void)
{
	if (isDrawArea) {
		SetUseLighting(false);
		collBack.Draw();
		collFront.Draw();
		banamerang.Draw();
		SetUseLighting(true);
	}
}

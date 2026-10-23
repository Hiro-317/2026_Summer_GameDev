#include "BananaBossBanamerang.h"


#include "../../../../../Common/CharacterBase/CharacterStatsDefine.h"

BananaBossBanamerang::BananaBossBanamerang(
	const MSG_SENDER_ID controlSenderId,
	const CharacterStats& stats
) :
	controlSenderId(controlSenderId),
	stats(stats)
{
	banamerang.LoadModel("Character/Banana/Banamerang");
	banamerang.scale = SIZE;
}

void BananaBossBanamerang::Load(void)
{
	// プレイヤー当たり判定を生成する（XZコライダー）
	collRef = new XZCircleCollider(COLLIDER_TAG::BossAttack, RADIUS, 120.0f);
	AddCollider(collRef);

	collRef->SetSkillStats(new SkillStats(85, &stats));
	SetPushFlg(false);
	SetJudgeFlg(false);

	ColliderSerch(GetColliders(), COLLIDER_TAG::BossAttack).back()->SetTransformPos(banamerang.pos);

	collBack.LoadModel("Range/CircleHalfRangeBack");
	collFront.LoadModel("Range/CircleHalfRangeFront");

	collBack.pos = POS;
	collFront.pos = POS;

	collBack.scale = SCALE;
	collFront.scale = Vector3(0.0f);

	collBack.localRotation = Quaternion::FromRotationY(Deg2Rad(180.0f));
	collFront.localRotation = Quaternion::FromRotationY(Deg2Rad(180.0f));

	isDrawArea = false;
	SetDrawType(ACTOR_DRAW_TYPE::Alpha);
}

void BananaBossBanamerang::SubDraw(void)
{
	if (isDrawArea) {
		SetUseLighting(false);
		collBack.Draw();
		collFront.Draw();
		banamerang.Draw();
		SetUseLighting(true);
	}
}

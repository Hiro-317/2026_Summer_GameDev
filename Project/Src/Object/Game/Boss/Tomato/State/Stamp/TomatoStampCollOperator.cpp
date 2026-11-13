#include "TomatoStampCollOperator.h"

#include "../../../../../Common/CharacterBase/CharacterStatsDefine.h"
#include "../../../../../Common/DataLoad/ParameterLoad.h"
#include "../../../../../Common/Collider/XZCircleCollider.h"


TomatoStampCollOperator::TomatoStampCollOperator
(
	float ATTACK_RADIUS,
	const bool& isGround,
	const MSG_SENDER_ID controlSenderId,
	const CharacterStats& stats,
	const ParameterLoad& collParam
)	: 
	ATTACK_RADIUS(ATTACK_RADIUS),
	ground(isGround),
	controlSenderId(controlSenderId),
	stats(stats),
	SCALE(collParam.GetParameterToVector3("Stamp","Scale")),
	HEIGHT(collParam.GetParameter("Stamp","Height")),
	collBack(), collFront()
{
	isDrawArea = false;
	trans.pos = Vector3(-1, -1, -1);
}

void TomatoStampCollOperator::Load(void)
{
	trans.pos = Vector3();

	// 動的オブジェクトとしての挙動を無効にする
	SetDynamicFlg(true);

	// 重力を無効にする
	SetGravityFlg(false);

	// 衝突時の押し出しを無効にする
	SetPushFlg(false);

#pragma endregion

	// 当たり判定情報を生成する
	AddCollider(new XZCircleCollider(COLLIDER_TAG::BossAttackArea, ATTACK_RADIUS, 1000.0f));
	collRef = new XZCircleCollider(COLLIDER_TAG::BossAttack, ATTACK_RADIUS, 1000.0f);
	AddCollider(collRef);
	SetJudgeFlg(false);

	// 攻撃範囲の当たり判定
	ColliderSerch(GetColliders(), COLLIDER_TAG::BossAttackArea).back()->SetJudgeFlg(true);

	collRef->SetSkillStats(new SkillStats(100, &stats));

	collBack.LoadModel("Range/CircleRangeBack");
	collFront.LoadModel("Range/CircleRangeFront");

	collBack.pos = Vector3::Yonly(HEIGHT);
	collFront.pos = Vector3::Yonly(HEIGHT + 1.0f);

	collBack.scale = SCALE;
	collFront.scale = Vector3(0);

	SetDrawType(ACTOR_DRAW_TYPE::Alpha);
}

void TomatoStampCollOperator::SubUpdate(void)
{
	// 押し戻された場所にすべて移動させる
	auto pos = ColliderSerch(GetColliders(), COLLIDER_TAG::BossAttackArea).back()->GetPos();
	ColliderSerch(GetColliders(), COLLIDER_TAG::BossAttack).back()->SetTransformPos(pos);
	collBack.pos = pos;
	collFront.pos = pos;
	trans.pos.y = HEIGHT;
}

void TomatoStampCollOperator::SubDraw(void)
{
	if (isDrawArea) {
		SetUseLighting(false);
		collBack.Draw();
		collFront.Draw();
		SetUseLighting(true);
	}
}

void TomatoStampCollOperator::SubRelease(void)
{
	collFront.Release();
	collBack.Release();
}

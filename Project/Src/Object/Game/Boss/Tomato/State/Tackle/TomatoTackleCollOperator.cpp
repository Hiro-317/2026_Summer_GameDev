#include "TomatoTackleCollOperator.h"

#include "../../../../../Common/DataLoad/ParameterLoad.h"

#include "../../../../../Common/Collider/XZCircleCollider.h"

#include "../../../../../Common/CharacterBase/CharacterStatsDefine.h"

TomatoTackleCollOperator::TomatoTackleCollOperator(
	const float TO_PLAYER_DISTANCE,

	const MSG_SENDER_ID& controlSenderId,

	const CharacterStats& stats,

	const ParameterLoad& collParam
) :

	TO_PLAYER_DISTANCE(TO_PLAYER_DISTANCE + 5.0f),

	controlSenderId(controlSenderId),

	stats(stats),

	HEIGHT(collParam.GetParameter("Tackle", "Height")),
	SCALE(collParam.GetParameterToVector3("Tackle", "Scale")),
	CENTER(collParam.GetParameterToVector3("Tackle", "Center")* SCALE),
	ANGLE(collParam.GetParameterToVector3("Tackle", "Angle")),

	attackCollRef(nullptr),
	stagePushCollRef(nullptr),

	collBack(),
	collFront(),

	stageHit(false),

	rate(),

	isDrawArea(false)
{
}

void TomatoTackleCollOperator::Load(void)
{
	// 当たり判定を生成する（XZコライダー）
	attackCollRef = new XZCircleCollider(COLLIDER_TAG::BOSS_ATTACK, TO_PLAYER_DISTANCE, 1000.0f);
	attackCollRef->SetSkillStats(new SkillStats(150, &stats));
	AddCollider(attackCollRef);

	// ステージ当たり判定を生成する（XZコライダー）
	stagePushCollRef = new XZCircleCollider(COLLIDER_TAG::BOSS_ATTACK_AREA, TO_PLAYER_DISTANCE, 1000.0f);
	AddCollider(stagePushCollRef);

	stagePushCollRef->SetJudgeFlg(true);
	attackCollRef->SetJudgeFlg(false);

#pragma region 攻撃範囲のモデル制御情報設定

	// モデル読み込み
	collBack.LoadModel("Range/LineRangeBack");
	collFront.LoadModel("Range/LineRangeFront");

	// 座標
	collBack.pos = Vector3::Yonly(HEIGHT);
	collFront.pos = Vector3::Yonly(HEIGHT);

	// スケール
	collBack.scale = SCALE;
	collFront.scale = Vector3(SCALE.x, SCALE.y, 0.0f);

	// モデルのズレ
	collBack.centerDiff = CENTER;
	collFront.centerDiff = CENTER + Vector3::Yonly(1.0f);

	// 回転
	collBack.localRotation = Quaternion::FromEulerXZY(ANGLE);
	collFront.localRotation = Quaternion::FromEulerXZY(ANGLE);

#pragma endregion

	isDrawArea = false;
}

void TomatoTackleCollOperator::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)
{
	// ステージに当たったか
	if (other.GetTag() == COLLIDER_TAG::STAGE && other.GetShape() == COLLIDER_SHAPE::XzCircle) {

		stageHit = true;
	}
}

void TomatoTackleCollOperator::SubDraw(void)
{
	if (isDrawArea) {
		SetUseLighting(false);
		collFront.Draw();
		collBack.Draw();
		SetUseLighting(true);
	}
}

void TomatoTackleCollOperator::SubRelease(void)
{
	collFront.Release();
	collBack.Release();
}

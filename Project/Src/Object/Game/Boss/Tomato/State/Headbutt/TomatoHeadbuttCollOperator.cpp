#include "TomatoHeadbuttCollOperator.h"

#include "../../../../../Common/Collider/XZCircleCollider.h"

TomatoHeadbuttCollOperator::TomatoHeadbuttCollOperator(
	const float TO_PLAYER_DISTANCE,
	const MSG_SENDER_ID controlSenderId,
	const CharacterStats& stats,
	const ParameterLoad& collParam
) :
	controlSenderId(controlSenderId),
	stats(stats),
	TO_PLAYER_DISTANCE(TO_PLAYER_DISTANCE + 5.0f),
	HEIGHT(collParam.GetParameter("Wahoo", "Height")),
	SCALE(collParam.GetParameterToVector3("Wahoo", "Scale")),
	CENTER(collParam.GetParameterToVector3("Wahoo", "Center")* SCALE),
	ANGLE(collParam.GetParameterToVector3("Wahoo", "Angle")),
	collBack(),
	collFront()
{
}

void TomatoHeadbuttCollOperator::Load(void)
{
	// プレイヤー当たり判定を生成する（XZコライダー）
	AddCollider(
		new XZCircleCollider(COLLIDER_TAG::BossAttack, TO_PLAYER_DISTANCE, 1000.0f)
	);

	CreateAttackSkill(controlSenderId, 75, &stats, COLLIDER_TAG::BossAttack);

	SetJudgeFlg(false);

	collBack.Load("Range/LineRangeBack");
	collFront.Load("Range/LineRangeFront");

	collBack.pos = Vector3::Yonly(HEIGHT);
	collFront.pos = Vector3::Yonly(HEIGHT);

	collBack.scale = SCALE;
	collFront.scale = Vector3(SCALE.x, SCALE.y, 0.0f);

	collBack.centerDiff = CENTER;
	collFront.centerDiff = CENTER + Vector3::Yonly(1.0f);

	collBack.localRotation = Quaternion::FromEulerXZY(ANGLE);
	collFront.localRotation = Quaternion::FromEulerXZY(ANGLE);

	isDrawArea = false;
}

void TomatoHeadbuttCollOperator::SubAlphaDraw(void)
{
	if (isDrawArea) {
		SetUseLighting(false);
		collFront.Draw();
		collBack.Draw();
		SetUseLighting(true);
	}
}

void TomatoHeadbuttCollOperator::SubRelease(void)
{
	collFront.Release();
	collBack.Release();
}

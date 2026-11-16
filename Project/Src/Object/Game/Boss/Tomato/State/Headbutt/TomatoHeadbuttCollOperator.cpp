#include "TomatoHeadbuttCollOperator.h"

#include "../../../../../Common/CharacterBase/CharacterStatsDefine.h"
#include "../../../../../Common/DataLoad/ParameterLoad.h"
#include "../../../../../Common/Collider/XZCircleCollider.h"

TomatoHeadbuttCollOperator::TomatoHeadbuttCollOperator(
	const float TO_PLAYER_DISTANCE,
	const MSG_SENDER_ID controlSenderId,
	const CharacterStats& stats,
	const ParameterLoad& attackRangeParameters
) :
	controlSenderId(controlSenderId),
	stats(stats),
	TO_PLAYER_DISTANCE(TO_PLAYER_DISTANCE + 5.0f),
	HEIGHT(attackRangeParameters.GetParameter("Wahoo", "Height")),
	SCALE(attackRangeParameters.GetParameterToVector3("Wahoo", "Scale")),
	CENTER(attackRangeParameters.GetParameterToVector3("Wahoo", "Center")* SCALE),
	ANGLE(attackRangeParameters.GetParameterToVector3("Wahoo", "Angle")),
	collBack(),
	collFront()
{
}

void TomatoHeadbuttCollOperator::Load(void)
{
	// プレイヤー当たり判定を生成する（XZコライダー）
	collRef = new XZCircleCollider(COLLIDER_TAG::BossAttack, TO_PLAYER_DISTANCE, 1000.0f);
	AddCollider(collRef);

	collRef->SetSkillStats(new SkillStats(75, &stats));

	SetJudgeFlg(false);

	collBack.LoadModel("Range/LineRangeBack");
	collFront.LoadModel("Range/LineRangeFront");

	collBack.pos = Vector3::Yonly(HEIGHT);
	collFront.pos = Vector3::Yonly(HEIGHT);

	collBack.scale = SCALE;
	collFront.scale = Vector3(SCALE.x, SCALE.y, 0.0f);

	collBack.centerDiff = CENTER;
	collFront.centerDiff = CENTER + Vector3::Yonly(1.0f);

	collBack.localRotation = Quaternion::FromEulerXZY(ANGLE);
	collFront.localRotation = Quaternion::FromEulerXZY(ANGLE);

	isDrawArea = false;
	SetDrawType(ACTOR_DRAW_TYPE::Alpha);
}

void TomatoHeadbuttCollOperator::SubDraw(void)
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

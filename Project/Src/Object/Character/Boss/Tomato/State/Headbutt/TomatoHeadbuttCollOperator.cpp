#include "TomatoHeadbuttCollOperator.h"

#include "../../../../../Common/Collider/XZCircleCollider.h"

TomatoHeadbuttCollOperator::TomatoHeadbuttCollOperator(
	const float TO_PLAYER_DISTANCE,
	const CharacterStats stats,
	const ParameterLoad& collParam
)
	: stats(stats),
	TO_PLAYER_DISTANCE(TO_PLAYER_DISTANCE + 5.0f),
	HEIGHT(collParam.GetParameter("Wahoo", "Height")),
	SCALE(collParam.GetParameterToVector3("Wahoo", "Scale")),
	CENTER(collParam.GetParameterToVector3("Wahoo", "Center")* SCALE),
	ANGLE(collParam.GetParameterToVector3("Wahoo", "Angle")),
	collBack(Vector3()),
	collFront(Vector3())
{
}

void TomatoHeadbuttCollOperator::Load(void)
{
	// プレイヤー当たり判定を生成する（XZコライダー）
	ColliderCreate(
		new XZCircleCollider(COLLIDER_TAG::BOSS_ATTACK, TO_PLAYER_DISTANCE)
	);

	CreateAttackSkill(75, &stats, COLLIDER_TAG::BOSS_ATTACK);

	SetJudge(false);

	collBack.Load("Range/LineRangeBack");
	collFront.Load("Range/LineRangeFront");

	collBack.pos = Vector3::Yonly(HEIGHT);
	collFront.pos = Vector3::Yonly(HEIGHT);

	collBack.scale = SCALE;
	collFront.scale = Vector3(SCALE.x, SCALE.y, 0.0f);

	collBack.centerDiff = CENTER;
	collFront.centerDiff = CENTER + Vector3::Yonly(1.0f);

	collBack.localAngle = ANGLE;
	collFront.localAngle = ANGLE;

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

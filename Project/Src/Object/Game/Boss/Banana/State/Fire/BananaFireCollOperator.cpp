#include "BananaFireCollOperator.h"

#include "../../../../../Common/CharacterBase/CharacterStatsDefine.h"


BananaFireCollOperator::BananaFireCollOperator(
	const CharacterStats& stats
) :
	stats(stats)
{
}

void BananaFireCollOperator::Load(void)
{
	// プレイヤー当たり判定を生成する（球体コライダー）
	collRef = new XZCircleCollider(COLLIDER_TAG::BossAttack, RADIUS, HEIGHT);
	AddCollider(collRef);

	collRef->SetSkillStats(new SkillStats(100, &stats));
	SetPushFlg(true);
	SetJudgeFlg(false);
	SetPushWeight(255);

	ColliderSerch(GetColliders(), COLLIDER_TAG::BossAttack).back()->SetTransformPos(POS);

	collBack.LoadModel("Range/CircleRangeBack");
	collFront.LoadModel("Range/CircleRangeFront");

	collBack.pos = POS;
	collFront.pos = POS;

	collBack.scale = SCALE;
	collFront.scale = Vector3(0.0f);

	isDrawArea = false;
	SetDrawType(ACTOR_DRAW_TYPE::Alpha);

	FlameBreathSettings fbSettings;
	fbSettings.color = 0xffff00;
	fbSettings.intensity = 1.0f;
	fbSettings.durationSeconds = 0.0f;
	fbSettings.radius = RADIUS;
	fbSettings.distance = (START_POS - POS).Length();
	flRef = new FlameBreathActor(START_POS, DIRECTION, POS, RADIUS, Vector3::Yonly(1.0f), fbSettings);
	AddChildActor(flRef);
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

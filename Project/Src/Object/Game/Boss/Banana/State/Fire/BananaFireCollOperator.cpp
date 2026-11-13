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

	FlameBreathSettings fbSettings;
	fbSettings.color = 0xffff00;
	fbSettings.intensity = 1.0f;
	fbSettings.durationSeconds = 0.0f;
	fbSettings.radius = 500.0f;
	fbSettings.distance = 1250.0f;
	flRef = new FlameBreathActor(Vector3::YZonly(500.0f, 1350.0f), Vector3::YZonly(-0.25f, -0.5f), fbSettings);
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

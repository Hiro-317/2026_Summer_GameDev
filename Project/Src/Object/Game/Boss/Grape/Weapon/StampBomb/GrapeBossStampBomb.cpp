#include "GrapeBossStampBomb.h"

#include "../../../../../../Manager/TimeScale/TimeScale.h"

#include "../../../../../Common/Collider/SphereCollider.h"

#include "../../../../../Common/CharacterBase/CharacterStatsDefine.h"

GrapeBossStampBomb::GrapeBossStampBomb(int model, int front, int back) :
	GrapeBossWeaponBase(model, front, back),

	state(STATE::None),

	objectCollRef(nullptr),
	attackCollRef(nullptr)

{
	// 地形との当たり判定用
	objectCollRef = new SphereCollider(COLLIDER_TAG::BossCommon, RADIUS);
	AddCollider(objectCollRef);

	// 当たり判定を生成する（球体コライダー）
	attackCollRef = new SphereCollider(COLLIDER_TAG::BossAttack, ATTACK_RADIUS);
	AddCollider(attackCollRef);

	trans.scale = Vector3(MODEL_SCALE);

	SetGravityFlg(true);
	SetPushFlg(false);
	
	count = 0;
}

void GrapeBossStampBomb::Load(const MSG_SENDER_ID controlSenderId, const CharacterStats& stats)
{
	attackCollRef->SetSkillStats(new SkillStats(75, &stats));

	const float ATTACK_RADIUS_SCALE = ATTACK_RADIUS / MODEL_RADIUS;
	collBack.scale = Vector3::XZonly(ATTACK_RADIUS_SCALE, ATTACK_RADIUS_SCALE);
	collFront.scale = Vector3(0.0f);
}

void GrapeBossStampBomb::SubUpdate(void)
{
	switch (state) {

	case GrapeBossStampBomb::STATE::None: { return; }

	case GrapeBossStampBomb::STATE::Fall: {

		// 接地判定
		if (isGround) { state = STATE::Ground; }

		break;
	}

	case GrapeBossStampBomb::STATE::Ground: {


		count += TimeScale::Get();

		// まだ爆発しない
		if (count < BOMBER_COUNT) {
			// カウントに応じて攻撃範囲の演出
			SetViewScaleCircle((ATTACK_RADIUS / MODEL_RADIUS) * (count / BOMBER_COUNT));
		}
		// 爆発
		else {
			state = STATE::Blast;
		}

		break;
	}

	case GrapeBossStampBomb::STATE::Blast: {

		state = STATE::None;

		break;
	}

	}
}

void GrapeBossStampBomb::Blast(bool send)
{
	SetViewScaleCircle(0.0f);

	objectCollRef->SetJudgeFlg(false);

	attackCollRef->SetJudgeFlg(true);

	SetGravityFlg(false);
	SetPushFlg(false);

}

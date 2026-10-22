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
	// 当たり判定を生成する（球体コライダー）
	AddCollider(new SphereCollider(COLLIDER_TAG::BossAttack, RADIUS));

	trans.scale = Vector3(MODEL_SCALE);

	SetGravityFlg(true);
	SetPushFlg(true);
	
	count = 0;
}

void GrapeBossStampBomb::Load(const MSG_SENDER_ID controlSenderId, const CharacterStats& stats)
{
	GetColliders().back()->SetSkillStats(new SkillStats(75, &stats));

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

	// 地面についてないなら加速して落ちる
	if (!isGround) {

		trans.pos.y -= 8.0f;
	}
	// ついてるなら爆発カウントを進める
	else {
		// 前フレームの判定が残ってるので
		// それ防止に
		// 開始されたフレームの検知
		if (!now) {
			// 今開始されたらフラグを立てる
			now = true;
			// ほんとはついてないので落下
			trans.pos.y -= 8.0f;
			return;
		}
		// 攻撃判定オン
		else if ((BOMBER_COUNT + ATTACK_DURATION) >= count) {
			GetColliders().back()->SetJudgeFlg(true);
		}
		//攻撃判定オフ、終わり
		else {
			GetColliders().back()->SetJudgeFlg(false);
			end = true;
			now = false;
			count = 0;
			SetViewScaleCircle(0.0f);
		}
	}
}
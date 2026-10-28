#include "GrapeBossStalkerBomb.h"

#include "../../../../../Common/Collider/SphereCollider.h"
#include "../../../../../Common/Collider/XZCircleCollider.h"

#include "../../../../../Common/CharacterBase/CharacterStatsDefine.h"

GrapeBossStalkerBomb::GrapeBossStalkerBomb(int model, int front, int back)
	: GrapeBossWeaponBase(model, front, back)
{
	SetPushFlg(true);

	// 当たり判定を生成する（XZコライダー）
	AddCollider(
		new XZCircleCollider(
			COLLIDER_TAG::BossAttack,
			ATTACK_RANGE,
			1000.0f
		)
	);

	// 当たり判定を生成する（球体コライダー）
	AddCollider(
		new SphereCollider(
			COLLIDER_TAG::BossAttackArea,
			MODEL_RADIUS * ATTACK_SIZE
		)
	);
	trans.scale = Vector3(ATTACK_SIZE);

	ColliderSerch(GetColliders(), COLLIDER_TAG::BossAttackArea).back()->SetJudgeFlg(true);
	ColliderSerch(GetColliders(), COLLIDER_TAG::BossAttack).back()->SetJudgeFlg(false);
	ColliderSerch(GetColliders(), COLLIDER_TAG::BossAttack).back()->SetPushFlg(false);
	SetGravityFlg(true);
	count = 0;
}

void GrapeBossStalkerBomb::Load(const MSG_SENDER_ID operatorSenderId, const CharacterStats& stats)
{
	ColliderSerch(GetColliders(), COLLIDER_TAG::BossAttack).back()->SetSkillStats(new SkillStats(75, &stats));

	const float ATTACK_RANGE = this->ATTACK_RANGE / this->MODEL_RADIUS;
	collBack.scale = Vector3::XZonly(ATTACK_RANGE, ATTACK_RANGE);
	collFront.scale = Vector3(0.0f);
}

void GrapeBossStalkerBomb::SubUpdate(void)
{
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
		count++;
		// カウントに応じて状態を変える
		if (BOMBER_COUNT >= count) {

			SetViewScaleCircle((ATTACK_RANGE / MODEL_RADIUS) * (count / BOMBER_COUNT));
		}
		// 攻撃判定オン
		else if ((BOMBER_COUNT + ATTACK_DURATION) >= count) {
			ColliderSerch(GetColliders(), COLLIDER_TAG::BossAttack).back()->SetJudgeFlg(true);
		}
		//攻撃判定オフ、終わり
		else {
			ColliderSerch(GetColliders(), COLLIDER_TAG::BossAttack).back()->SetJudgeFlg(false);
			end = true;
			now = false;
			count = 0;
			SetViewScaleCircle(0.0f);
		}
	}
}

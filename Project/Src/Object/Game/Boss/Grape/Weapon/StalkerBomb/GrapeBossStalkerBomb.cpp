#include "GrapeBossStalkerBomb.h"

#include "../../../../../../Manager/TimeScale/TimeScale.h"

#include "../../../../../Common/Collider/SphereCollider.h"

#include "../../../../../Common/CharacterBase/CharacterStatsDefine.h"

GrapeBossStalkerBomb::GrapeBossStalkerBomb(int model, int front, int back)
	: GrapeBossWeaponBase(model, front, back)
{
	// 当たり判定を生成する（球体コライダー）
	AddCollider(new SphereCollider(COLLIDER_TAG::BossAttack, RADIUS));

	trans.scale = Vector3(MODEL_SCALE);

	SetGravityFlg(true);
	SetPushFlg(true);
	count = 0;
}

void GrapeBossStalkerBomb::Load(const MSG_SENDER_ID controlSenderId, const CharacterStats& stats)
{
	GetColliders().back()->SetSkillStats(new SkillStats(75, &stats));

	const float ATTACK_RADIUS_SCALE = ATTACK_RADIUS / MODEL_RADIUS;
	collBack.scale = Vector3::XZonly(ATTACK_RADIUS_SCALE, ATTACK_RADIUS_SCALE);
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
			ColliderSerch(GetCollider(), COLLIDER_TAG::BossAttack).back()->SetJudgeFlg(true);
		}
		//攻撃判定オフ、終わり
		else {
			ColliderSerch(GetCollider(), COLLIDER_TAG::BossAttack).back()->SetJudgeFlg(false);
			end = true;
			now = false;
			count = 0;
			SetViewScaleCircle(0.0f);
			// ホストのみエフェクト再生
			if (Net::GetIns().IsHost()) {
				EffectManager::GetIns()->CreateEffect(EFFECT_NAME::BOMB_SMALL, trans.pos);
			}
		}
	}
}

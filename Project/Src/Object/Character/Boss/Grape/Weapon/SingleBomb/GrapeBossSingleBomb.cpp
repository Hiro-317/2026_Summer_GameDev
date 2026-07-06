#include "GrapeBossSingleBomb.h"

#include "../../../../../Common/Collider/SphereCollider.h"
#include "../../../../../Common/Collider/XZCircleCollider.h"


GrapeBossSingleBomb::GrapeBossSingleBomb(int model)
	: GrapeBossWeaponBase("path", model)
{
	// 当たり判定を生成する（XZコライダー）
	ColliderCreate(
		new XZCircleCollider(
			COLLIDER_TAG::BOSS_ATTACK,
			GetParameter("SingleBomb", "Radius")
		)
	);

	// 当たり判定を生成する（球体コライダー）
	ColliderCreate(
		new SphereCollider(
			COLLIDER_TAG::BOSS_ATTACK_AREA,
			GetParameter("SingleBomb", "Radius")
		)
	);

	ColliderSerch(COLLIDER_TAG::BOSS_ATTACK).back()->SetJudgeFlg(false);
}

void GrapeBossSingleBomb::Load(const MSG_SENDER_ID operatorSenderId, const CharacterStats& stats)
{
	CreateAttackSkill(operatorSenderId, 50, &stats);

	collBack.scale = Vector3::XZonly(ATTACK_RANGE, ATTACK_RANGE);
	collFront.scale = Vector3(0.0f);
}

void GrapeBossSingleBomb::SubUpdate(void)
{
	// 地面についてないなら加速して落ちる
	if (!isGround) {

		trans.pos.y -= 3.0f;
	}
	// ついてるなら爆発カウントを進める
	else {
		count++;
		// カウントに応じて状態を変える
		if (BOMBER_COUNT <= count) {

			SetViewScaleCircle(ATTACK_RANGE * (BOMBER_COUNT / count));
		}
		// 攻撃判定オン
		else if ((BOMBER_COUNT + ATTACK_DURATION) <= count) {
			ColliderSerch(COLLIDER_TAG::BOSS_ATTACK).back()->SetJudgeFlg(true);
		}
		//攻撃判定オフ、終わり
		else {
			ColliderSerch(COLLIDER_TAG::BOSS_ATTACK).back()->SetJudgeFlg(false);
			end = true;
		}
	}
}

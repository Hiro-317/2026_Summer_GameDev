#include "GrapeBossKickBomb.h"

#include "../../../../../Common/Collider/XZCircleCollider.h"


GrapeBossKickBomb::GrapeBossKickBomb(int model)
	: GrapeBossWeaponBase("path", model)
{
	// 当たり判定を生成する（球体コライダー）
	ColliderCreate(
		new XZCircleCollider(
			COLLIDER_TAG::BOSS_ATTACK,
			GetParameter("KickBomb", "Radius")
		)
	);
	SetJudge(false);
}

void GrapeBossKickBomb::Load(const MSG_SENDER_ID operatorSenderId, const CharacterStats& stats)
{
	CreateAttackSkill(operatorSenderId, 50, &stats);

	collBack.scale = ATTACK_RANGE;
	collFront.scale = Vector3(0.0f);
}

void GrapeBossKickBomb::SubUpdate(void)
{
	if (!isGround) {

		trans.pos.y -= 3.0f;
	}
	if (BOMBER_COUNT <= count) {

		count++;
		SetViewScaleCircle(count);
	}
	else if((BOMBER_COUNT + ATTACK_DURATION) <= count){
		SetJudge(true);
	}
	else {
		SetJudge(false);
		end = true;
	}
}

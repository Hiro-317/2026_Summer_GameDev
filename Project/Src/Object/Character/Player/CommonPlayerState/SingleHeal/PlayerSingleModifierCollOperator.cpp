#include "PlayerSingleModifierCollOperator.h"

#include "../../../../Common/Collider/SphereCollider.h"

PlayerSingleModifierCollOperator::PlayerSingleModifierCollOperator(
	COLLIDER_TAG COLL_TAG, 
	MSG_SENDER_ID operatorSenderId,
	const Vector3* const& targetPos,
	const short SKILL_POWER,
	const short SKILL_TIME
) : 
	COLL_TAG(COLL_TAG),
	operatorSenderId(operatorSenderId),
	targetPos(targetPos),
	SKILL_POWER(SKILL_POWER),
	SKILL_TIME(SKILL_TIME)
{
}

void PlayerSingleModifierCollOperator::Load(void)
{
#pragma region 基底クラスにある機能の挙動設定

	// 動的オブジェクトとしての挙動を無効にする
	SetDynamicFlg(true);

	// 重力を無効にする
	SetGravityFlg(false);

	// 衝突時の押し出しを無効にする
	SetPushFlg(false);

#pragma endregion
	// 効果時間が未設定(-1)の場合、回復スキルとみなす
	if (SKILL_TIME == -1) { CreateHealSkill(operatorSenderId, SKILL_POWER, COLL_TAG); }
	else { CreateModifierSkill(operatorSenderId, SKILL_POWER, SKILL_TIME, COLL_TAG); }

	// 当たり判定情報を生成する
	ColliderCreate(new SphereCollider(COLL_TAG, COLL_RADIUS));
	SetJudge(false);
}

void PlayerSingleModifierCollOperator::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const Vector3& collisionPoint)
{
	switch (other.GetTag())
	{
	case COLLIDER_TAG::PLAYER:
		isHit = true;
		break;
	default:break;
	}
}

void PlayerSingleModifierCollOperator::SubUpdate(void)
{
	if (GetJudgeFlg()) {
		if (isHit) { CollOff(); } 
		trans.pos = *targetPos;
	}
}


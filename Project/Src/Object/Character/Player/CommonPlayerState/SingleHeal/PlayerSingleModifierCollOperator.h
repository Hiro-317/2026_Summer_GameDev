#pragma once

#include "../../../../ActorBase.h"
#include "../../../../../Manager/Net/NetWorkManager.h"

class PlayerSingleModifierCollOperator : public ActorBase
{
public:

	PlayerSingleModifierCollOperator(
		COLLIDER_TAG COLL_TAG,
		MSG_SENDER_ID operatorSenderId,
		const short SKILL_POWER,
		const short SKILL_TIME = -1
	);
	~PlayerSingleModifierCollOperator()override = default;

	void Load(void)override;

	void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other)override;

	// 攻撃の判定を発生させる
	void CollOn(void) {
		if (!isHit)ColliderSerch(COLL_TAG).back()->SetJudgeFlg(true);
		if (!Net::GetIns().IsHost()) {
			Net::GetIns().Send(
				MsgDataPlayerCollOperator(true, MsgDataPlayerCollOperator::COLLIDER_KINDS::CommonPlayerSingleModifier),
				operatorSenderId
			);
		}
	}

	// 攻撃の判定を消す
	void CollOff(void) {
		ColliderSerch(COLL_TAG).back()->SetJudgeFlg(false);
		if (!Net::GetIns().IsHost()) {
			Net::GetIns().Send(
				MsgDataPlayerCollOperator(false, MsgDataPlayerCollOperator::COLLIDER_KINDS::CommonPlayerSingleModifier),
				operatorSenderId
			);
		}
	}

	// 座標指定
	void SetTargetPos(const Vector3& targetPos) {
		trans.pos = targetPos;
	}

	// 攻撃のヒット管理のフラグをリセットする
	void ResetIsHit(void) { isHit = false; }
private:

#pragma region 定数

	const COLLIDER_TAG COLL_TAG;

	const float COLL_RADIUS = 1.0f;

	// 効果量
	const short SKILL_POWER;

	// 効果時間
	const short SKILL_TIME;

#pragma endregion

#pragma region 受け取る参照変数


	const MSG_SENDER_ID operatorSenderId;

#pragma endregion

	// 攻撃のヒット管理のフラグ
	bool isHit;

	void SubUpdate(void)override;
};
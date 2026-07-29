#pragma once
#include "../../../../ActorBase.h"

#include "../../../../../Manager/Net/NetWorkManager.h"

class PlayerAscensionCollOperator : public ActorBase 
{
public:
	PlayerAscensionCollOperator(
		COLLIDER_TAG COLL_TAG,
		// プレイヤーの座標
		const Vector3& playerPos,
		// 昇天を阻止できる範囲の半径
		const float PREVENTION_RADIUS,
		// 送信ID
		MSG_SENDER_ID operatorSenderId
		);
	~PlayerAscensionCollOperator()override = default;

	void Load(void)override;
	void Update(void)override;

	// 攻撃の判定を発生させる
	void CollOn(void) {
		ColliderSerch(COLL_TAG).back()->SetJudgeFlg(true);
		if (!Net::GetIns().IsHost()) {
			Net::GetIns().Send(MsgDataPlayerCollOperator(true, MsgDataPlayerCollOperator::COLLIDER_TYPE::CommonPlayerAscention));
		}
	}

	// 攻撃の判定を消す
	void CollOff(void) {
		ColliderSerch(COLL_TAG).back()->SetJudgeFlg(false);
		if (!Net::GetIns().IsHost()) {
			Net::GetIns().Send(MsgDataPlayerCollOperator(false, MsgDataPlayerCollOperator::COLLIDER_TYPE::CommonPlayerAscention));
		}
	}


private:

	COLLIDER_TAG COLL_TAG;

	const Vector3& playerPos;	// プレイヤーの座標

	const float PREVENTION_RADIUS;	// 昇天を阻止できる範囲の半径

	MSG_SENDER_ID operatorSenderId;		// 送信ID
};

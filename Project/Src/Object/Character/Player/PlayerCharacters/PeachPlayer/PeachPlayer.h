#pragma once

#include "../../PlayerBase.h"

#include "../../../../../Common/Vector2.h"

class PeachPlayer : public PlayerBase
{
public:

	PeachPlayer(MSG_SENDER_ID operatorSenderId);
	~PeachPlayer()override = default;

	void PlayerLoad(void)override;

	// 当たり判定処理
	void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other)override;

	void ReceptionUpdate(void)override;
	void SendUpdate(void)override;

private:

#pragma region スキル1の定数定義


#pragma endregion

#pragma region スキル2の定数定義


#pragma endregion 

#pragma region スキル3の定数定義


#pragma endregion 
};
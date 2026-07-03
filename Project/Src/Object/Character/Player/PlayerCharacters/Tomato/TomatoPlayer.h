#pragma once

#include "../../PlayerBase.h"

#include "../../../../../Common/Vector2.h"

class TomatoPlayer : public PlayerBase
{
public:

	TomatoPlayer(MSG_SENDER_ID operatorSenderId);
	~TomatoPlayer()override = default;

	void PlayerLoad(void)override;

	void ReceptionUpdate(void)override;
	void SendUpdate(void)override;

private:

#pragma region スキル１の定数定義

	// クールタイムの時間
	const int SKILL2_COOL_TIME = GetParameterToInt("Tackle", "CoolTime");

	// 突進速度
	const float MOVE_SPEED = GetParameter("Tackle", "MoveSpeed");;
	
	// 回転速度
	const float ROTATION_POW = Deg2Rad(GetParameter("Tackle", "RotationPower"));

	// 攻撃倍率
	const short SKILL2_DAMAGE_RATE = (short)GetParameter("Tackle", "DamageRate");

#pragma endregion 

};
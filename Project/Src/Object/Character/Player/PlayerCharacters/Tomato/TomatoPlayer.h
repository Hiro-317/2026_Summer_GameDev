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
	// 攻撃倍率
	const short SKILL2_DAMAGE_RATE = (short)GetParameter("Tackle", "DamageRate");

	// 突進速度
	const float TACKLE_SPEED = GetParameter("Tackle", "MoveSpeed");;
	
	// 回転速度
	const float ROTATION_POW = Deg2Rad(GetParameter("Tackle", "RotationPower"));

#pragma endregion 

#pragma region スキル２の定数定義

	// クールタイムの時間
	const int SKILL1_COOL_TIME = GetParameterToInt("HeadButt", "CoolTime");
	// 攻撃倍率
	const short SKILL1_DAMAGE_RATE = (short)GetParameter("HeadButt", "DamageRate");

	// 突進速度
	const float HEAD_BUTT_SPEED = GetParameter("HeadButt", "AttackSpeed");

	// 頭突き中の時間
	const float HEAD_BUTT_ATTACK_TIME = GetParameter("HeadButt", "AttackTime");

#pragma endregion
};
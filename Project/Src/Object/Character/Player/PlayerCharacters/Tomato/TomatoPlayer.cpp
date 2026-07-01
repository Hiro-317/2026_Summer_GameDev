#include "TomatoPlayer.h"

#include "UniqueState/TomatoPlayerMoveState.h"

TomatoPlayer::TomatoPlayer(MSG_SENDER_ID operatorSenderId) :
	PlayerBase(
		operatorSenderId,

		"TomatoParameter",
		"PlayerHP",
		"PlayerAttackPower",
		"PlayerDefensePower",
		"PlayerMoveSpeed",

		"Data/Parameter/Character/Player/Tomato/",
		"Tomato/Tomato")
{
}

void TomatoPlayer::PlayerLoad(void)
{


#pragma region 状態設定
	
	// 移動状態を追加する
	AddState(
		(int)STATE::MOVE,
		new TomatoPlayerMoveState(
			// 自分の状態に遷移する関数
			[&]() { ChangeState((int)STATE::MOVE); },
			// 自分の状態かどうかを返す関数
			[&]() { return state == (int)STATE::MOVE; },
			// 定数（加算移動量 / 移動量の最大値 / ダッシュの移動量倍率 / スタミナ量 / 加速減衰量）
			DASH_SPEED_RATE, DASH_STAMINA_MAX, ATTENUATION,
			// 参照（移動量 / 横軸加速度の最大値 / 角度）
			accelSum, ACCEL_MAX, trans.angle, characterStats
		)
	);

#pragma endregion 
}

void TomatoPlayer::ReceptionUpdate(void)
{
}

void TomatoPlayer::SendUpdate(void)
{
}

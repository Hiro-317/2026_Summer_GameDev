#pragma once

#include "../../../../Common/CharacterBase/CharacterStateBase.h"

#include "../../../../../Common/Vector3.h"
#include "../../../../../Common/Quaternion.h"

#include "../../../../../Manager/Input/InputManager.h"

class PlayerDodgeState : public CharacterStateBase
{
public:
	PlayerDodgeState(
		KEY_TYPE DODGE_KEY,
		int COOL_TIME, float DODGE_MOVE_SPEED, 
		float INVI_START_TIME, float INVI_END_TIME,

		const Quaternion& rotation,

		bool& isInvincible,
		
		std::function<void(void)> PlayDodgeAnime,
		std::function<float(void)> GetAnimePlayRatio,
		std::function<void(void)> DefaultChangeState
	);
	~PlayerDodgeState()override = default;
	void OwnStateConditionUpdate(void)override;
	void Enter(void)override;
	void Update(void)override;
	void Exit(void)override;
	void AlwaysUpdate(void)override;

private:

#pragma region 定数

	// 回避に使用するキー
	const KEY_TYPE DODGE_KEY;

	// 回避のクールタイム
	const int COOL_TIME;

	// 回避の移動速度
	const float DODGE_MOVE_SPEED;

	// 回避の無敵時間（アニメーションの再生割合）
	const float INVI_START_TIME;

	// 回避の無敵時間終了（アニメーションの再生割合）
	const float INVI_END_TIME;
#pragma	endregion 

#pragma region 受け取る参照変数・関数

	// 座標の参照
	Vector3& pos;
	// 角度の参照
	Quaternion& rotation;

	// 回避無敵判定
	bool& isInvincible;

	// 攻撃アニメーションの再生関数のポインタ
	const std::function<void(void)> PlayDodgeAnime;
	// アニメーションの再生割合を取得する関数のポインタ
	const std::function<float(void)> GetAnimePlayRatio;

	// 攻撃終了後の状態遷移関数のポインタ
	const std::function<void(void)> DefaultChangeState;

#pragma endregion

	// クールタイム計測
	int coolTimeCounter;
};
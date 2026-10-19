#pragma once

#include "../../../../Common/CharacterBase/CharacterStateBase.h"

#include "../../../../../Manager/Input/InputManager.h"

struct Vector3;
struct Quaternion;

struct CharacterStats;

class PlayerDodgeState : public CharacterStateBase
{
public:

	/// <summary>
	/// 回避
	/// </summary>
	/// <param name="DODGE_KEY">使用するキー</param>
	/// <param name="COOL_TIME">クールタイム</param>
	/// <param name="INVI_START_TIME">無敵判定発生開始時間</param>
	/// <param name="INVI_END_TIME">無敵判定発生終了時間</param>
	/// <param name="rotation">回転情報の参照</param>
	/// <param name="moveAccel">移動関数</param>
	/// <param name="isInvincible">無敵判定操作参照</param>
	/// <param name="playDodgeAnime">回避アニメーション再生関数</param>
	/// <param name="getAnimePlayRatio">アニメーション再生割合取得関数</param>
	/// <param name="defaultChangeState">回避行動終了後の状態遷移関数</param>
	PlayerDodgeState(
		KEY_TYPE DODGE_KEY,

		int COOL_TIME,

		float INVI_START_TIME,
		float INVI_END_TIME,

		const Quaternion& rotation,

		std::function<void(const Vector3&)> moveAccel,

		bool& isInvincible,
		
		std::function<void(void)> playDodgeAnime,
		std::function<float(void)> getAnimePlayRatio,
		std::function<void(void)> defaultChangeState
	);

	~PlayerDodgeState()override = default;

	void OwnStateConditionUpdate(void)override;
	void Enter(void)override;
	void Update(void)override;
	void Exit(void)override;
	void AlwaysUpdate(void)override;

	// クールタイムを参照
	const float& GetCoolTimeCounter(void)const { return coolTimeCounter; }

private:

#pragma region 定数

	// 回避に使用するキー
	const KEY_TYPE DODGE_KEY;

	// 回避のクールタイム
	const int COOL_TIME;

	// 回避の無敵時間（アニメーションの再生割合）
	const float INVI_START_TIME;

	// 回避の無敵時間終了（アニメーションの再生割合）
	const float INVI_END_TIME;

#pragma	endregion 

#pragma region 受け取る参照変数・関数

	// 角度
	const Quaternion& rotation;

	// 移動関数のポインタ
	const std::function<void(const Vector3&)> moveAccel;

	// 回避無敵判定
	bool& isInvincible;

	// 回避アニメーションの再生関数のポインタ
	const std::function<void(void)> playDodgeAnime;
	// アニメーションの再生割合を取得する関数のポインタ
	const std::function<float(void)> getAnimePlayRatio;

	// 攻撃終了後の状態遷移関数のポインタ
	const std::function<void(void)> defaultChangeState;

#pragma endregion

	// クールタイム計測
	int coolTimeCounter;
};
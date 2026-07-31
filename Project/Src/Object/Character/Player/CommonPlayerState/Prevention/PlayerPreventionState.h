#pragma once

#include "../../../CharacterStateBase.h"

#include "../../../../../Common/Vector3.h"

#include "PlayerPreventionCollOperator.h"

class PlayerPreventionState : public CharacterStateBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="ownChangeState">自分の状態に遷移する関数</param>
	/// <param name="isOwnState">自分の状態かどうかを返す関数</param>
	/// <param name="accelSum">移動量の参照</param>
	/// <param name="angle">角度の参照</param>
	/// <param name="PlayIdleAnime">待機アニメーションの再生関数のポインタ</param>
	/// <param name="PlayWalkAnime">歩きアニメーションの再生関数のポインタ</param>
	/// <param name="PlayRunAnime">走りアニメーションの再生関数のポインタ</param>
	PlayerPreventionState(
		const std::function<void(void)>& ownChangeState,
		const std::function<bool(void)>& isOwnState,
		Vector3& accelSum, Vector3& angle, const CharacterStats& playerStats,
		const std::function<void(void)>& PlayIdleAnime,
		const std::function<void(void)>& PlayWalkAnime,
		const std::function<void(void)>& PlayRunAnime,
		PlayerPreventionCollOperator& collOperator
	);

	~PlayerPreventionState()override = default;

	// 自分の状態に遷移する条件関数
	void OwnStateConditionUpdate(void);

	// 状態遷移後1度行う初期化処理
	void Enter(void)override;
	// 更新処理
	void Update(void)override;
	// 状態遷移前1度行う終了処理
	void Exit(void)override;

private:

	void Move(void);

#pragma region 定数

	// 横軸加速度の最大値の参照
	float ACCEL_MAX = playerStats.speedPower.Value();

#pragma endregion

#pragma region 受け取る参照変数・関数

	// 移動量参照
	Vector3& accelSum;

	// 角度の参照
	Vector3& angle;

	const CharacterStats& playerStats;

	// 待機アニメーションの再生関数のポインタ
	const std::function<void(void)> PlayIdleAnime;
	// 歩きアニメーションの再生関数のポインタ
	const std::function<void(void)> PlayWalkAnime;
	// 走りアニメーションの再生関数のポインタ
	const std::function<void(void)> PlayRunAnime;

	// 当たり判定管理クラス
	PlayerPreventionCollOperator& collOperator;

#pragma endregion
};
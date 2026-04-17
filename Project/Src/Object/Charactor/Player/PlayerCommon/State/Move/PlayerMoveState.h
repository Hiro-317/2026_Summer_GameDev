#pragma once

#include <functional>

#include "../../../../CharactorStateBase.h"

#include "../../../../../../Common/Vector3.h"

class PlayerMoveState : public CharactorStateBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="ownChangeState">自分の状態に遷移する関数</param>
	/// <param name="isOwnState">自分の状態かどうかを返す関数</param>
	/// <param name="accelSum">移動量の参照</param>
	/// <param name="ACCEL_MAX">横軸加速度の最大値の参照</param>
	/// <param name="angle">角度の参照</param>
	/// <param name="MOVE_SPEED">加算移動量</param>
	/// <param name="MOVE_SPEED_MAX">移動量の最大値</param>
	/// <param name="DASH_SPEED_RATE">ダッシュの移動量倍率</param>
	/// <param name="DASH_STAMINA_MAX">ダッシュのスタミナの最大量（1フレームずつデクリメント）</param>
	/// <param name="PlayIdleAnime">待機アニメーションの再生関数のポインタ</param>
	/// <param name="PlayWalkAnime">歩きアニメーションの再生関数のポインタ</param>
	/// <param name="PlayRunAnime">走りアニメーションの再生関数のポインタ</param>
	PlayerMoveState(
		const std::function<void(void)>& ownChangeState,
		std::function<bool(void)> isOwnState,
		Vector3& accelSum, float& ACCEL_MAX, Vector3& angle,
		float MOVE_SPEED, float MOVE_SPEED_MAX, float DASH_SPEED_RATE, short DASH_STAMINA_MAX,
		const std::function<void(void)> PlayIdleAnime, const std::function<void(void)> PlayWalkAnime, const std::function<void(void)> PlayRunAnime
	);
	~PlayerMoveState()override = default;

	// 自分の状態に遷移する条件関数
	void OwnStateConditionUpdate(void);

	// 状態遷移後1度行う初期化処理
	void Enter(void)override;
	// 更新処理
	void Update(void)override;
	// 状態遷移前1度行う終了処理
	void Exit(void)override;

	// 状態関係なく呼び出す処理
	void AlwaysUpdate(void)override;

private:

#pragma region 受け取る参照変数・関数

	// 移動量参照
	Vector3& accelSum;

	// 横軸加速度の最大値の参照
	float& ACCEL_MAX;

	// 角度の参照
	Vector3& angle;

	// 待機アニメーションの再生関数のポインタ
	const std::function<void(void)> PlayIdleAnime;
	// 歩きアニメーションの再生関数のポインタ
	const std::function<void(void)> PlayWalkAnime;
	// 走りアニメーションの再生関数のポインタ
	const std::function<void(void)> PlayRunAnime;

#pragma endregion

#pragma region 定数

	// 加算移動量
	const float MOVE_SPEED;

	// 最大移動量
	const float MOVE_SPEED_MAX;

	// ダッシュの移動量倍率
	const float DASH_SPEED_RATE;

	// ダッシュのスタミナの最大量（1フレームずつデクリメント）
	const short DASH_STAMINA_MAX;

#pragma endregion

	// ダッシュしているかどうか
	bool isDash;

	// ダッシュスタミナ
	short dashStamina;

	// 息切れ(1度スタミナが0になったかどうか)
	bool isTired;
};
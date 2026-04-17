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
	/// <param name="ownChangeState">自分の状態に遷移する関数 </param>
	/// <param name="isOwnState">自分の状態かどうかを返す関数 </param>
	/// <param name="accelSum">移動量の参照 </param>
	/// <param name="angle">角度の参照 </param>
	/// <param name="ADD_MOVE_SPEED">加算移動量 </param>
	/// <param name="DASH_SPEED_RATE">ダッシュの移動量倍率 </param>
	/// <param name="DASH_STAMINA_MAX">	ダッシュのスタミナの最大量（1フレームずつデクリメント） </param>
	/// <param name="PlayIdleAnime">待機アニメーションの再生関数のポインタ </param>
	/// <param name="PlayWalkAnime">歩きアニメーションの再生関数のポインタ </param>
	/// <param name="PlayRunAnime">走りアニメーションの再生関数のポインタ </param>
	PlayerMoveState(
		const std::function<void(void)>& ownChangeState,
		std::function<bool(void)> isOwnState,
		Vector3& accelSum, Vector3& angle,
		float ADD_MOVE_SPEED, float DASH_SPEED_RATE, short DASH_STAMINA_MAX,
		const std::function<void(void)> PlayIdleAnime, const std::function<void(void)> PlayWalkAnime, const std::function<void(void)> PlayRunAnime
	);
	~PlayerMoveState()override = default;

	// 自分の状態に遷移する条件関数
	void OwnStateConditionUpdate(void);

	void Init(void);
	void Update(void);
	void Exit(void);

private:

#pragma region 受け取る参照変数・関数

	// 移動量参照
	Vector3& accelSum;

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
	const float ADD_MOVE_SPEED;

	// ダッシュの移動量倍率
	const float DASH_SPEED_RATE;

	// ダッシュのスタミナの最大量（1フレームずつデクリメント）
	const short DASH_STAMINA_MAX;

#pragma endregion

	// ダッシュフラグ
	bool dashFlg;
	// ダッシュスタミナ
	short dashStamina;

};
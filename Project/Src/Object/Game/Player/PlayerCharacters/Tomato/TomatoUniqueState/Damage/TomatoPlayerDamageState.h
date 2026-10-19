#pragma once

#include "../../../../../../Common/CharacterBase/CharacterStateBase.h"

struct Quaternion;

class TomatoPlayerDamageState : public CharacterStateBase
{
public:

	/// <summary>
	/// ダメージ
	/// </summary>
	/// <param name="rotation"></param>
	/// <param name="InviStart"></param>
	/// <param name="DefaultChangeState"></param>
	TomatoPlayerDamageState(
		Quaternion& rotation,
		const std::function<void(void)> inviStart,
		const std::function<void(void)> defaultChangeState
	);

	// デストラクタ
	~TomatoPlayerDamageState()override = default;

	// 状態遷移後1度行う初期化処理
	void Enter(void)override;
	// 更新処理
	void Update(void)override;
	// 状態遷移前1度行う終了処理
	void Exit(void)override;

private:

	// プレイヤーの回転の参照
	Quaternion& rotation;

	// 無敵時間のセット関数
	const std::function<void(void)> inviStart;

	// ダメージ状態終了後の状態遷移関数のポインタ
	const std::function<void(void)> defaultChangeState;
};

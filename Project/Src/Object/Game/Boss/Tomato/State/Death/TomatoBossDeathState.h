#pragma once

#include "../../../../../Common/CharacterBase/CharacterStateBase.h"
#include "../../../../../../Common/Vector3.h"
#include "../../../../../../Common/Quaternion.h"

class TomatoBossDeathState : public CharacterStateBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="scale">ボスのスケール</param>
	/// <param name="defaultScale">ボスの初期スケール</param>
	/// <param name="IsDeath">アニメーション後の死んだ判定</param>
	TomatoBossDeathState(
		Vector3& scale, Quaternion& rotation,
		const std::function<void(void)> IsDeath
	);
	~TomatoBossDeathState()override = default;

	// 状態遷移後1度行う初期化処理
	void Enter(void)override;
	// 更新処理
	void Update(void)override;
	// 状態遷移前1度行う終了処理
	void Exit(void)override;

	// 状態関係なく呼び出す処理
	void AlwaysUpdate(void)override;

private:
#pragma region 参照関数

	// ボスのサイズ
	Vector3& scale;

	// ボスの角度
	Quaternion& rotation;

	// ボスの初期サイズ
	const Vector3& defaultScale;

	// 死亡判定移行
	const std::function<void(void)> IsDeath;

#pragma endregion 参照関数

#pragma region 定数定義

	static constexpr float CHANGE_COUNT = 120.0f;
	static constexpr float ROTATION_POW = 0.349066f;
	static constexpr float SCALE_POW = 0.975f;

#pragma endregion 定数定義

	float cnt;
};
#pragma once

#include "../../../../../Common/CharacterBase/CharacterStateBase.h"

class BananaBossDeathState : public CharacterStateBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="PlayDeath">死亡アニメーションを再生する。</param>
	/// <param name="MarkDead">死亡完了フラグを設定する。</param>
	BananaBossDeathState(
		const std::function<void(void)>& PlayDeath,
		const std::function<void(void)> MarkDead
	);
	~BananaBossDeathState()override = default;

	//// 自分の状態に遷移する条件関数
	//void OwnStateConditionUpdate(void);

	// 状態遷移後1度行う初期化処理
	void Enter(void)override;
	// 更新処理
	void Update(void)override;
	// 状態遷移前1度行う終了処理
	void Exit(void)override;

	// 状態関係なく呼び出す処理
	void AlwaysUpdate(void)override;

private:
	bool hasNotifiedDeath = false;
#pragma region 参照関数

	// 死亡判定移行
	const std::function<void(void)> PlayDeath;
	const std::function<void(void)> MarkDead;

#pragma endregion 参照関数

#pragma region 定数定義

	static constexpr int CHANGE_COUNT = 120;

#pragma endregion 定数定義

	int elapsedFrames;
};
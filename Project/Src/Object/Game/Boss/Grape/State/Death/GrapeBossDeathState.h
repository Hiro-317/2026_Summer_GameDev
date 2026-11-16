#pragma once

#include "../../../../../Common/CharacterBase/CharacterStateBase.h"

class GrapeBossDeathState : public CharacterStateBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="MarkDead">死亡完了フラグを設定する。</param>
	/// <param name="AnimeTotalTime">死亡アニメーションの長さを取得する。</param>
	/// <param name="IsAnimEnd">死亡アニメーションの終了判定を取得する。</param>
	/// <param name="DeathAnim">死亡アニメーションを再生する。</param>
	GrapeBossDeathState(
		const std::function<void(void)> DeathAnim,
		const std::function<float(void)> AnimeTotalTime,
		const std::function<bool(void)> IsAnimEnd,
		const std::function<void(void)> MarkDead
	);
	~GrapeBossDeathState()override = default;

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

	// 死亡アニメーション
	const std::function<void(void)> DeathAnim;

	// アニメーションの全体時間
	const std::function<float(void)> AnimeTotalTime;

	// アニメーション終了判定
	const std::function<bool(void)> IsAnimEnd;

	// 死亡判定移行
	const std::function<void(void)> MarkDead;

#pragma endregion 参照関数

#pragma region 定数定義

	static constexpr float SCALE_REVERSE = 22.0f;
	static constexpr float BOMB_RATE = 0.0020661157f;
	static constexpr int DEATH_LAST_TIME = 60;

#pragma endregion 定数定義

	int deathDelayFrames;

	int elapsedFrames;
};
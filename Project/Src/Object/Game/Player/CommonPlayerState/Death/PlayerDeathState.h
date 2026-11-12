#pragma once

#include "../../../../Common/CharacterBase/CharacterStateBase.h"

#include "../../../../../Common/Vector3.h"
#include "../../../../../Common/Quaternion.h"

class PlayerDeathState : public CharacterStateBase
{
public:
	
	/// <summary>
	/// 死亡状態
	/// </summary>
	/// <param name="playAnimeDeath">死亡アニメーション再生</param>
	/// <param name="isAnimeEnd">アニメーションが終了したかどうかを返す関数</param>
	/// <param name="defaultChangeState">死亡演出後の処理</param>
	PlayerDeathState(
		std::function<void(void)> playAnimeDeath,
		std::function<bool(void)> isAnimeEnd,
		std::function<void(void)> playerDeathSetting
	);

	~PlayerDeathState()override = default;

	// 状態遷移後1度行う初期化処理
	void Enter(void)override;
	// 更新処理
	void Update(void)override;
	// 状態遷移前1度行う終了処理
	void Exit(void)override;

private:

	// 死亡アニメーション
	const std::function<void(void)> playAnimeDeath;
	// アニメーションが終了したかのフラグを取得する関数のポインタ
	const std::function<bool(void)> isAnimeEnd;
	// 死亡後の設定
	const std::function<void(void)> playerDeathSetting;
};


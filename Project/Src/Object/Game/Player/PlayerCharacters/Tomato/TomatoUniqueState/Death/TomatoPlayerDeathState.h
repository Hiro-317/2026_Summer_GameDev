#pragma once

#include "../../../../../../Common/CharacterBase/CharacterStateBase.h"

struct Transform;

class TomatoPlayerDeathState : public CharacterStateBase
{
public:
	TomatoPlayerDeathState(Transform& playerTrans, std::function<void(void)> playerDeathSetting);

	~TomatoPlayerDeathState()override = default;

	// 状態遷移後1度行う初期化処理
	void Enter(void)override;
	// 更新処理
	void Update(void)override;
	// 状態遷移前1度行う終了処理
	void Exit(void)override;

private:

	// モデル制御情報の参照
	Transform& playerTrans;

	// 死亡後の設定
	const std::function<void(void)> playerDeathSetting;

	float animeCounter;
};


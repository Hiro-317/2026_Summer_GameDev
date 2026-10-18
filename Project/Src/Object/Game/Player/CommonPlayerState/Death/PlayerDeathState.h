#pragma once

#include "../../../../Common/CharacterBase/CharacterStateBase.h"

#include "../../../../../Common/Vector3.h"
#include "../../../../../Common/Quaternion.h"

class PlayerDeathState : public CharacterStateBase
{
public:
	PlayerDeathState(
		Vector3& pos, Quaternion& rotation,
		std::function<bool(void)> isAnimeEnd,
		std::function<void(void)> playDeathAnime,
		std::function<void(void)> playerDeathSetting,
		std::function<void(void)> defaultChangeState
	);

	~PlayerDeathState()override = default;

	// 状態遷移後1度行う初期化処理
	void Enter(void)override;
	// 更新処理
	void Update(void)override;
	// 状態遷移前1度行う終了処理
	void Exit(void)override;

private:

	// 座標の参照
	Vector3& pos;
	// 角度の参照
	Quaternion& rotation;

	// アニメーションが終了したかのフラグを取得する関数のポインタ
	const std::function<bool(void)> isAnimeEnd;
	// 死亡アニメーション
	const std::function<void(void)> playDeathAnime;
	// 死亡後の設定
	const std::function<void(void)> playerDeathSetting;
	// ダメージ状態終了後の状態遷移関数のポインタ
	const std::function<void(void)> defaultChangeState;
};


#pragma once
#include "../../../../Common/CharacterBase/CharacterStateBase.h"

#include "../../../../../Common/Vector3.h"

#include "../../../../../Manager/Input/InputManager.h"

#include "PlayerSingleModifierCollOperator.h"

class PlayerSingleModifierState : public CharacterStateBase
{
public:

	PlayerSingleModifierState(
		KEY_TYPE HEAL_KEY,

		int COOL_TIME,

		float COLL_START_TIME,

		PlayerSingleModifierCollOperator& collOperator,

		std::function<void(void)> playHealAnime,
		std::function<float(void)> getAnimePlayRatio,
		std::function<void(void)> defaultChangeState
	);
	~PlayerSingleModifierState() = default;

	// 自分の状態に遷移する条件関数
	void OwnStateConditionUpdate(void)override;

	// 状態遷移後1度行う初期化処理
	void Enter(void)override;
	// 更新処理
	void Update(void)override;
	// 状態遷移前1度行う終了処理
	void Exit(void)override;

	// 状態関係なく呼び出す処理
	void AlwaysUpdate(void)override;

private:

#pragma region 定数

	// 遷移するためのキー
	const KEY_TYPE HEAL_KEY;

	// 攻撃そのもののクールタイム
	const int COOL_TIME;

	// 攻撃の判定を発生させる開始時間（アニメーションの再生割合）
	const float COLL_START_TIME;

#pragma endregion


#pragma region 受け取る参照変数・関数

	// 当たり判定のオペレーターの参照
	PlayerSingleModifierCollOperator& collOperator;

	// 攻撃アニメーションの再生関数のポインタ
	const std::function<void(void)> playHealAnime;
	// アニメーションの再生割合を取得する関数のポインタ
	const std::function<float(void)> getAnimePlayRatio;
	// 攻撃終了後の状態遷移関数のポインタ
	const std::function<void(void)> defaultChangeState;

#pragma endregion

	int coolTimeCounter;
};
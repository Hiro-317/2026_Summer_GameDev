#include "PlayerDamageState.h"


PlayerDamageState::PlayerDamageState(
	const std::function<void(void)>& ownChangeState, 
	const std::function<bool(void)>& isOwnState,
	const std::function<void()> DefaultChangeState
) :
	CharactorStateBase(ownChangeState,isOwnState),
	DefaultChangeState(DefaultChangeState)
{
}

// 状態遷移後1度行う初期化処理
void PlayerDamageState::Enter(void)
{
}

// 更新処理
void PlayerDamageState::Update(void)
{
}

// 状態遷移前1度行う終了処理
void PlayerDamageState::Exit(void)
{
	// 移動処理に戻る
	if (nowDamageCounter <= 0) {
		DefaultChangeState();
	}
}

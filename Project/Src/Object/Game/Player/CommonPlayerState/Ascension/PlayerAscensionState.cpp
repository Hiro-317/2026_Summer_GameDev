#include "PlayerAscensionState.h"

PlayerAscensionState::PlayerAscensionState( std::function<void()> defaultChangeState) :
	defaultChangeState(defaultChangeState)
{
}

// 状態遷移後1度行う初期化処理
void PlayerAscensionState::Enter(void)
{
}

// 更新処理
void PlayerAscensionState::Update(void)
{
}

// 状態遷移前1度行う終了処理
void PlayerAscensionState::Exit(void)
{
}
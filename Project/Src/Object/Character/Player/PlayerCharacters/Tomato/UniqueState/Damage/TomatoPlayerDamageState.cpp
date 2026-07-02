#include "TomatoPlayerDamageState.h"

TomatoPlayerDamageState::TomatoPlayerDamageState(
	const std::function<void(void)>& ownChangeState,
	const std::function<bool(void)>& isOwnState,
	Vector3& pos,
	const std::function<void(void)> InviStart,
	const std::function<void(void)> DefaultChangeState
):
	CharacterStateBase(ownChangeState, isOwnState),
	InviStart(InviStart),
	DefaultChangeState(DefaultChangeState),
	pos(pos)
{
}

// 状態遷移後1度行う初期化処理
void TomatoPlayerDamageState::Enter(void)
{
	// 無敵カウンターを設定
	InviStart();
}

// 更新処理
void TomatoPlayerDamageState::Update(void)
{
	// 元の状態に遷移
	if (true) { DefaultChangeState(); }
}

// 状態遷移前1度行う終了処理
void TomatoPlayerDamageState::Exit(void)
{
}

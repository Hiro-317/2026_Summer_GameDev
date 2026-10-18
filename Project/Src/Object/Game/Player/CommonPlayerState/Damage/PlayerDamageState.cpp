#include "PlayerDamageState.h"

PlayerDamageState::PlayerDamageState(
	std::function<void(void)> playDamageAnime,
	std::function<bool(void)> isAnimeEnd,
	std::function<void()> inviStart,
	std::function<void()> defaultChangeState
) :
	playDamageAnime(playDamageAnime),
	isAnimeEnd(isAnimeEnd),
	inviStart(inviStart),
	defaultChangeState(defaultChangeState)
{
}

// 状態遷移後1度行う初期化処理
void PlayerDamageState::Enter(void)
{
	// アニメーション再生
	playDamageAnime();

	// 無敵カウンターを設定
	inviStart();
}

// 更新処理
void PlayerDamageState::Update(void)
{
	// 元の状態に遷移
	if (isAnimeEnd()) { defaultChangeState(); }
}

// 状態遷移前1度行う終了処理
void PlayerDamageState::Exit(void)
{
}

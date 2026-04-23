#include "PlayerDamageState.h"
// Windows デバッグ出力を使って簡易ログ
#include <Windows.h>

PlayerDamageState::PlayerDamageState(
	const std::function<void(void)>& ownChangeState, 
	const std::function<bool(void)>& isOwnState,
	int DAMAGE_INVI_START_TIME, int DAMAGE_INVI_END_TIME,
	const std::function<void(void)> PlayDamageAnime,
	const std::function<float(void)> GetAnimePlayRatio,
	const std::function<bool(void)> IsAnimeEnd,
	const std::function<void(unsigned char)> SetInviCounter,
	const std::function<void()> DefaultChangeState
) :
	CharactorStateBase(ownChangeState,isOwnState),
	DAMAGE_INVI_START_TIME(DAMAGE_INVI_START_TIME),
	DAMAGE_INVI_END_TIME(DAMAGE_INVI_END_TIME),
	PlayDamageAnime(PlayDamageAnime),
	GetAnimePlayRatio(GetAnimePlayRatio),
	IsAnimeEnd(IsAnimeEnd),
	SetInviCounter(SetInviCounter),
	DefaultChangeState(DefaultChangeState)
{
}

// 状態遷移後1度行う初期化処理
void PlayerDamageState::Enter(void)
{
    // デバッグ: Enter が呼ばれているか確認
	OutputDebugStringA("PlayerDamageState::Enter\n");			

	// アニメーションの再生
	if (PlayDamageAnime) {
	PlayDamageAnime();
		OutputDebugStringA("PlayerDamageState: PlayDamageAnime called\n");
	}
	else {
		OutputDebugStringA("PlayerDamageState: PlayDamageAnime is empty\n");
	}
}

// 更新処理
void PlayerDamageState::Update(void)
{
	float animePlayRate = GetAnimePlayRatio();

	if (
		DAMAGE_INVI_START_TIME <= animePlayRate &&
		animePlayRate <= DAMAGE_INVI_END_TIME
		) 
	{
		SetInviCounter(1);
	}

	if (IsAnimeEnd()) { DefaultChangeState(); }
}

// 状態遷移前1度行う終了処理
void PlayerDamageState::Exit(void)
{

}

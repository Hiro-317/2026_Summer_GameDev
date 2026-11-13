#include "PlayerSingleModifierState.h"

#include "../../../../../Manager/TimeScale/TimeScale.h"
#include "../../../../../Manager/Sound/SoundManager.h"

#include "PlayerSingleModifierCollOperator.h"

PlayerSingleModifierState::PlayerSingleModifierState(
	KEY_TYPE HEAL_KEY,

	int COOL_TIME,

	float COLL_START_TIME,

	PlayerSingleModifierCollOperator& collOperator,

	std::function<void(void)> playHealAnime,
	std::function<float(void)> getAnimePlayRatio,
	std::function<void(void)> defaultChangeState
) :
	HEAL_KEY(HEAL_KEY),

	COOL_TIME(COOL_TIME),

	COLL_START_TIME(COLL_START_TIME),

	collOperator(collOperator),

	playHealAnime(playHealAnime),
	getAnimePlayRatio(getAnimePlayRatio),
	defaultChangeState(defaultChangeState),

	coolTimeCounter(),
	isPlaySound(false)
{
}

void PlayerSingleModifierState::OwnStateConditionUpdate(void)
{
	// 攻撃のクールタイム中は状態遷移しない
	if (coolTimeCounter > 0.0f) { return; }

	// 攻撃キーのダウントリガーで状態遷移
	if (Input::GetIns().GetInfo(HEAL_KEY).down) { OwnChangeState(); }
}

void PlayerSingleModifierState::Enter(void)
{
	// クールタイムをセット
	coolTimeCounter = (float)COOL_TIME;

	// 判定管理のリセット
	collOperator.CollOff();
	collOperator.ResetIsHit();

	// アニメーション再生
	playHealAnime();

	// サウンド再生フラグをリセット
	isPlaySound = false;
}

void PlayerSingleModifierState::Update(void)
{
	if (getAnimePlayRatio() > COLL_START_TIME) {

		if (!isPlaySound) {
			isPlaySound = true;
			Snd::GetIns().Play("PlayerHeal");
		}

		collOperator.CollOn();

		if (getAnimePlayRatio() >= 1.0f) { defaultChangeState(); }
	}
}

void PlayerSingleModifierState::Exit(void)
{
	collOperator.CollOff();
	collOperator.ResetIsHit();
}

void PlayerSingleModifierState::AlwaysUpdate(void)
{
	// 自身の状態でないときは、攻撃のクールタイムを減らす
	if (!IsOwnState()) {
		// 攻撃のクールタイムを減らす
		if (coolTimeCounter > 0.0f) { 
			coolTimeCounter -= TimeScale::Get();
			if (coolTimeCounter <= 0.0f) { coolTimeCounter = 0.0f; }
		}
	}
}
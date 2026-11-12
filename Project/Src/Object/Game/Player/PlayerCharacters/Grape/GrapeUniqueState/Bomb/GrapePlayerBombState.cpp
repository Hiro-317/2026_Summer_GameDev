#include "GrapePlayerBombState.h"

#include "../../../../../../../Manager/Input/InputManager.h"

#include "GrapePlayerWeaponBomb.h"

GrapePlayerBombState::GrapePlayerBombState(
	const int COOL_TIME,

	GrapePlayerWeaponBomb& bombWeapon,

	Transform& playerTrans,

	const std::function<void(void)> playAnime,
	const std::function<float(void)> getAnimeRatio,
	const std::function<void(void)> defaultChangeState
) :
	COOL_TIME(COOL_TIME),
	bombWeapon(bombWeapon),
	playerTrans(playerTrans),
	playAnime(playAnime),
	getAnimeRatio(getAnimeRatio),
	defaultChangeState(defaultChangeState),

	isShot(),
	coolTimeCounter()
{
}

void GrapePlayerBombState::OwnStateConditionUpdate(void)
{
	// 攻撃のクールタイム中は状態遷移しない
	if (coolTimeCounter > 0) { return; }

	// ステート遷移処理
	if (Input::GetIns().GetInfo(KEY_TYPE::PlayerSkill3).down) {
		OwnChangeState();
	}
}

void GrapePlayerBombState::Enter(void)
{
	// クールタイムをセット
	coolTimeCounter = COOL_TIME;

	// アニメーションを再生
	playAnime();
}

void GrapePlayerBombState::Update(void)
{
	const float animeRatio = getAnimeRatio();

	if (animeRatio > 0.3f) {
		if (!isShot) {
			// 発射
			bombWeapon.Put(playerTrans.pos);
			isShot = true;
		}
	}

	if (animeRatio >= 1.0f) { defaultChangeState(); }
}

void GrapePlayerBombState::AlwaysUpdate(void)
{
	// 自身の状態でないときは、攻撃のクールタイムを減らす
	if (!IsOwnState()) {
		// 攻撃のクールタイムを減らす
		if (coolTimeCounter > 0.0f) {
			coolTimeCounter -= TimeScale::Get();
			if (coolTimeCounter < 0.0f) { coolTimeCounter = 0.0f; }
		}
	}
}

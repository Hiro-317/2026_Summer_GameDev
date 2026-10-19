#include "PlayerDodgeState.h"

#include "../../../../../Common/Vector3.h"
#include "../../../../../Common/Quaternion.h"

#include "../../../../../Manager/TimeScale/TimeScale.h"

PlayerDodgeState::PlayerDodgeState(
	KEY_TYPE DODGE_KEY,
	
	int COOL_TIME,

	float INVI_START_TIME,
	float INVI_END_TIME,

	const Quaternion& rotation,

	std::function<void(const Vector3&)> moveAccel,
	bool& isInvincible,
	
	std::function<void(void)> playDodgeAnime,
	std::function<float(void)> getAnimePlayRatio,
	std::function<void(void)> defaultChangeState
):
	DODGE_KEY(DODGE_KEY),

	COOL_TIME(COOL_TIME),

	INVI_START_TIME(INVI_START_TIME),
	INVI_END_TIME(INVI_END_TIME),

	rotation(rotation),

	moveAccel(moveAccel),

	isInvincible(isInvincible),

	playDodgeAnime(playDodgeAnime),
	getAnimePlayRatio(getAnimePlayRatio),
	defaultChangeState(defaultChangeState),

	coolTimeCounter()
{
}

void PlayerDodgeState::OwnStateConditionUpdate(void)
{
	// 攻撃のクールタイム中は状態遷移しない
	if (coolTimeCounter > 0.0f) { return; }

	// 攻撃キーのダウントリガーで状態遷移
	if (Input::GetIns().GetInfo(DODGE_KEY).down) { OwnChangeState(); }
}

void PlayerDodgeState::Enter(void)
{
	// アニメーションの再生
	playDodgeAnime();

	// 攻撃のクールタイムをセットする
	coolTimeCounter = (float)COOL_TIME;
}

void PlayerDodgeState::Update(void)
{
	// 無敵判定終了前移動させる
	if (getAnimePlayRatio() <= INVI_END_TIME) {
		// 回避の移動処理
		moveAccel(rotation.Rotate(Vector3::Zonly(1.0f)));
	}

	// アニメーションが終了していたら指定の通常状態に遷移させる
	if (getAnimePlayRatio() >= 1.0f) { defaultChangeState(); }
}

void PlayerDodgeState::Exit(void)
{

}

void PlayerDodgeState::AlwaysUpdate(void)
{
	// 自身の状態でないときクールタイムを減らす
	if (!IsOwnState()) {
		// 攻撃のクールタイムを減らす
		if (coolTimeCounter > 0.0f) {
			coolTimeCounter -= TimeScale::Get();
			if (coolTimeCounter <= 0.0f) { coolTimeCounter = 0.0f; }
		}
	}
}

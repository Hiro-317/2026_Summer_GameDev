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

	step(STEP::Startup),
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

	// ステップを「無敵判定発生前」へ
	step = STEP::Startup;

	// 攻撃のクールタイムをセットする
	coolTimeCounter = (float)COOL_TIME;

	// 無敵判定を消去
	isInvincible = false;
}

void PlayerDodgeState::Update(void)
{
	// アニメーションの再生割合を取得
	const float animeRatio = getAnimePlayRatio();

	// ステップ別更新
	switch (step) {

	case STEP::Startup: {
		// 前隙

		// 移動ベクトル
		Vector3 moveVec = {};

		// 回避の移動処理
		moveAccel(rotation.Rotate(Vector3::Zonly(1.0f)));

		// 無敵判定発生開始
		if (INVI_START_TIME <= animeRatio) {

			// ステップを「無敵判定発生中」へ
			step = STEP::Active;

			// 無敵判定を発生
			isInvincible = true;
		}

		break;
	}

	case STEP::Active: {
		// 攻撃判定発生中

		// 攻撃判定発生終了
		if (INVI_END_TIME <= animeRatio) {

			// ステップを「後隙」へ
			step = STEP::Recovery;

			// 無敵判定を消去
			isInvincible = false;
		}

		break;
	}

	case STEP::Recovery: {
		// 後隙

		// アニメーション再生終了で強制的に待機状態へ
		if (1.0f <= animeRatio) {

			// 待機状態へ遷移
			defaultChangeState();
		}

		break;
	}

	}
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

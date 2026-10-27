#include "PlayerSimpleAttackState.h"

#include "../../../../../Manager/TimeScale/TimeScale.h"
#include "../../../../../Manager/Sound/SoundManager.h"

#include "PlayerSimpleAttackCollOperator.h"

PlayerSimpleAttackState::PlayerSimpleAttackState(
	KEY_TYPE ATTACK_KEY,

	int COOL_TIME,

	float COLL_START_TIME,
	float COLL_END_TIME,

	PlayerSimpleAttackCollOperator& collOperator,

	const Transform& trans,

	std::function<void(const Vector3&)> moveAccel,

	std::function<void(void)> playAttackAnime,
	std::function<float(void)> getAnimePlayRatio,
	std::function<void(void)> defaultChangeState
):
	ATTACK_KEY(ATTACK_KEY),

	COOL_TIME(COOL_TIME),

	COLL_START_TIME(COLL_START_TIME),
	COLL_END_TIME(COLL_END_TIME),

	collOperator(collOperator),

	trans(trans),

	moveAccel(moveAccel),

	playAttackAnime(playAttackAnime),
	getAnimePlayRatio(getAnimePlayRatio),
	defaultChangeState(defaultChangeState),

	step(STEP::Startup),
	coolTimeCounter()
{
}

void PlayerSimpleAttackState::OwnStateConditionUpdate(void)
{
	// 攻撃のクールタイム中は状態遷移しない
	if (coolTimeCounter > 0.0f) { return; }

	// 攻撃キーのダウントリガーで状態遷移
	if (Input::GetIns().GetInfo(ATTACK_KEY).down) { OwnChangeState(); }
}

void PlayerSimpleAttackState::Enter(void)
{
	// ステップを「前隙」へ
	step = STEP::Startup;


	// 攻撃のクールタイムをセットする
	coolTimeCounter = (float)COOL_TIME;


	// 当たり判定を消去
	collOperator.CollOff();

	// 攻撃対象の探索を開始する
	collOperator.TargetSerch();

	// 攻撃のヒット管理のフラグをリセット
	collOperator.ResetIsHit();


	// 攻撃アニメーションを再生する
	playAttackAnime();

	// 効果音
	SoundManager::GetIns().Play("PlayerSimpleAttack");
}

void PlayerSimpleAttackState::Update(void)
{
	// アニメーションの再生割合を取得
	const float animeRatio = getAnimePlayRatio();

	// ステップ別更新
	switch (step) {

	case STEP::Startup: {
		// 前隙

		// 移動ベクトル
		Vector3 moveVec = {};

		if (collOperator.GetIsFindAttackTarget()) {
			// 攻撃対象が見つかっていれば、攻撃対象に向かって移動する
			moveVec = collOperator.GetTargetPos() - trans.pos;
			moveVec.y = 0.0f;
			moveVec.Normalize();
		}
		else {
			// 攻撃対象が見つかっていなければ、角度の方向に移動する
			moveVec = trans.rotation.Rotate(Vector3::Zonly(1.0f));
			moveVec.y = 0.0f;
			moveVec.Normalize();
		}

		// 最終的な移動ベクトルで移動する
		moveAccel(moveVec);

		// 攻撃判定発生開始
		if (COLL_START_TIME <= animeRatio) {

			// ステップを「攻撃判定発生中」へ
			step = STEP::Active;

			// 当たり判定を発生
			collOperator.CollOn();
		}

		break;
	}

	case STEP::Active: {
		// 攻撃判定発生中

		// 攻撃判定発生終了
		if (animeRatio <= COLL_END_TIME) {

			// ステップを「後隙」へ
			step = STEP::Recovery;

			// 当たり判定を消去
			collOperator.CollOff();
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

void PlayerSimpleAttackState::Exit(void)
{
	// 当たり判定をオフにする
	collOperator.CollOff();

	// 探索情報をリセットする
	collOperator.ResetTarget();
}

void PlayerSimpleAttackState::AlwaysUpdate(void)
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
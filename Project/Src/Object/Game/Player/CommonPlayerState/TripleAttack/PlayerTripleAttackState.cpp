#include "PlayerTripleAttackState.h"

#include "../../../../../Manager/Sound/SoundManager.h"

#include "PlayerTripleAttackCollOperator.h"

PlayerTripleAttackState::PlayerTripleAttackState(
	KEY_TYPE ATTACK_KEY,

	int COOL_TIME,

	int ATTACK_NEXT_STAGE_CONTINUE_TIME,

	std::array<float, (size_t)PLAYER_TRIPLE_ATTACK_STAGE::Max> COLL_START_TIME,
	std::array<float, (size_t)PLAYER_TRIPLE_ATTACK_STAGE::Max> COLL_END_TIME,

	PlayerTripleAttackCollOperator& collOperator,

	const Transform& playerTrans,

	std::function<void(const Vector3&)> moveAccel,

	std::array<std::function<void(void)>, (size_t)PLAYER_TRIPLE_ATTACK_STAGE::Max> playAttackAnimes,
	std::function<float(void)> getAnimePlayRatio,

	std::function<void(void)> defaultChangeState
):
	ATTACK_KEY(ATTACK_KEY),

	COOL_TIME(COOL_TIME),

	ATTACK_NEXT_STAGE_CONTINUE_TIME(ATTACK_NEXT_STAGE_CONTINUE_TIME),

	COLL_START_TIME(COLL_START_TIME),
	COLL_END_TIME(COLL_END_TIME),

	collOperator(collOperator),

	playerTrans(playerTrans),

	moveAccel(moveAccel),

	playAttackAnimes(playAttackAnimes),

	getAnimePlayRatio(getAnimePlayRatio),

	defaultChangeState(defaultChangeState),

	step(STEP::Startup),

	coolTimeCounter(),

	attackStage(PLAYER_TRIPLE_ATTACK_STAGE::None),
	attackNextStageContinueTimeCounter(0)
{
}

void PlayerTripleAttackState::OwnStateConditionUpdate(void)
{
	// 攻撃のクールタイム中は状態遷移しない
	if (coolTimeCounter > 0) { return; }

	// 攻撃キーのダウントリガーで状態遷移
	if (Input::GetIns().GetInfo(ATTACK_KEY).down) { OwnChangeState(); }
}

void PlayerTripleAttackState::Enter(void)
{
	// 攻撃段数を進める
	attackStage = (PLAYER_TRIPLE_ATTACK_STAGE)((int)attackStage + 1);

	// 最終段までいってるか確かめる
	if (attackStage == (PLAYER_TRIPLE_ATTACK_STAGE)((int)PLAYER_TRIPLE_ATTACK_STAGE::Max - 1)) {

		// 最終段なら、攻撃のクールタイムをセットする
		coolTimeCounter = COOL_TIME;

		// 次段に繋がるまでのカウンターをセットする
		attackNextStageContinueTimeCounter = 0;
	}

	// 攻撃対象の探索を開始する
	collOperator.TargetSerch();
	// 攻撃のヒット管理のフラグをリセットする
	collOperator.ResetIsHit();
	// 前回までの当たり判定を消す
	collOperator.CollOff();

	// 次段に繋がるまでのカウンターをセットする
	attackNextStageContinueTimeCounter = ATTACK_NEXT_STAGE_CONTINUE_TIME;

	// 攻撃段数に応じた攻撃アニメーションを再生する
	playAttackAnimes[(int)attackStage]();

	// 効果音
	SoundManager::GetIns().Play("PlayerSimpleAttack");
}

void PlayerTripleAttackState::Update(void)
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
			moveVec = collOperator.GetTargetPos() - playerTrans.pos;
		}
		else {
			// 攻撃対象が見つかっていなければ、角度の方向に移動する
			moveVec = playerTrans.rotation.Rotate(Vector3::Zonly(1.0f));
		}

		// 最終的な移動ベクトルで移動する
		moveAccel(moveVec);

		// 攻撃判定発生開始
		if (COLL_START_TIME[(int)attackStage] <= animeRatio) {

			// ステップを「攻撃判定発生中」へ
			step = STEP::Active;

			// 当たり判定を発生
			collOperator.CollOn(attackStage);
		}

		break;
	}

	case STEP::Active: {
		// 攻撃判定発生中

		// 攻撃判定発生終了
		if (animeRatio <= COLL_END_TIME[(int)attackStage]) {

			// ステップを「後隙」へ
			step = STEP::Recovery;

			// 当たり判定を消去
			collOperator.CollOff(attackStage);
		}

		break;
	}

	case STEP::Recovery: {
		// 後隙

		// 入力によって次段に繋がるようにしておく
		OwnStateConditionUpdate();

		// アニメーション再生終了で強制的に待機状態へ
		if (1.0f <= animeRatio) {

			// 待機状態へ遷移
			defaultChangeState();
		}

		break;
	}

	}
}

void PlayerTripleAttackState::Exit(void)
{
	// 当たり判定をオフにする
	collOperator.CollOff();
	// 探索情報をリセットする
	collOperator.ResetTarget();
}

void PlayerTripleAttackState::AlwaysUpdate(void)
{
	// 自身の状態でないときは、攻撃のクールタイムと次段に繋がるまでのカウンターを減らす
	if (!IsOwnState()) {
		// 攻撃のクールタイムを減らす
		if (coolTimeCounter > 0) { 
			coolTimeCounter--;

			if(coolTimeCounter <= 0) {
				// 攻撃段数をリセットする
				attackStage = PLAYER_TRIPLE_ATTACK_STAGE::None;
			}
		}

		// 次段に繋がるまでのカウンターを減らす
		if (attackNextStageContinueTimeCounter > 0) {
			attackNextStageContinueTimeCounter--;

			// 猶予時間がなくなったら
			if (attackNextStageContinueTimeCounter <= 0) {

				// 攻撃段数をリセットする
				attackStage = PLAYER_TRIPLE_ATTACK_STAGE::None;

				// 攻撃のクールタイムをセットする
				coolTimeCounter = COOL_TIME;
			}
		}
	}

	// 探索情報をリセットする
	collOperator.ResetTarget();
}

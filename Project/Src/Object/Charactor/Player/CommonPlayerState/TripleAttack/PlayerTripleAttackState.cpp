#include "PlayerTripleAttackState.h"

#include "../../../../../Manager/Input/KeyManager.h"

PlayerTripleAttackState::PlayerTripleAttackState(
	const std::function<void(void)>& ownChangeState,
	const std::function<bool(void)>& isOwnState,

	int COOL_TIME,
	int ATTACK_NEXT_STAGE_CONTINUE_TIME,
	float COLL_START_TIME_1, float COLL_END_TIME_1,
	float COLL_START_TIME_2, float COLL_END_TIME_2,
	float COLL_START_TIME_3, float COLL_END_TIME_3,
	float ATTACK_MOVE_SPEED,

	PlayerTripleAttackCollOperator& collOperator,
	Vector3& pos, Vector3& angle,

	const std::function<void(void)>& PlayAttack1Anime,
	const std::function<void(void)>& PlayAttack2Anime,
	const std::function<void(void)>& PlayAttack3Anime,

	const std::function<float(void)>& GetAnimePlayRatio,
	const std::function<bool(void)>& IsAnimeEnd,

	const std::function<void(void)>& DefaultChangeState
) :
CharactorStateBase(ownChangeState, isOwnState),

COOL_TIME(COOL_TIME),
ATTACK_NEXT_STAGE_CONTINUE_TIME(ATTACK_NEXT_STAGE_CONTINUE_TIME),
COLL_START_TIME(COLL_START_TIME_1, COLL_START_TIME_2, COLL_START_TIME_3),
COLL_END_TIME(COLL_END_TIME_1, COLL_END_TIME_2, COLL_END_TIME_3),
ATTACK_MOVE_SPEED(ATTACK_MOVE_SPEED),

collOperator(collOperator),
pos(pos), angle(angle),

PlayAttackAnimes{ PlayAttack1Anime, PlayAttack2Anime, PlayAttack3Anime },
GetAnimePlayRatio(GetAnimePlayRatio),
IsAnimeEnd(IsAnimeEnd),

DefaultChangeState(DefaultChangeState),

attackStage(PLAYER_TRIPLE_ATTACK_STAGE::NON),
coolTimeCounter(0),
attackNextStageContinueTimeCounter(0)
{
}

void PlayerTripleAttackState::OwnStateConditionUpdate(void)
{
	// 攻撃のクールタイム中は状態遷移しない
	if (coolTimeCounter > 0) { return; }

	// 攻撃キーのダウントリガーで状態遷移
	if (Key::GetIns().GetInfo(KEY_TYPE::PLAYER_ATTACK).down) {
		OwnChangeState();
		Enter();
	}
}

void PlayerTripleAttackState::Enter(void)
{
	// 攻撃段数を進める
	attackStage = (PLAYER_TRIPLE_ATTACK_STAGE)((int)attackStage + 1);

	// 攻撃段数に応じた攻撃アニメーションを再生する
	PlayAttackAnimes[(int)attackStage]();

	// 最終段なら、攻撃のクールタイムをセットする
	if (attackStage == PLAYER_TRIPLE_ATTACK_STAGE::THIRD) { coolTimeCounter = COOL_TIME; }

	// 次段に繋がるまでのカウンターをセットする
	attackNextStageContinueTimeCounter = ATTACK_NEXT_STAGE_CONTINUE_TIME;

	// 攻撃対象の探索を開始する
	collOperator.TargetSerch();
}

void PlayerTripleAttackState::Update(void)
{
	// 一旦攻撃判定をオフにしておく
	collOperator.CollOff();

	// アニメーションの再生割合を取得する
	float animePlayRate = GetAnimePlayRatio();
	
	if (animePlayRate < COLL_START_TIME[(int)attackStage]) {
		// 攻撃判定開始前

		// 移動ベクトル
		Vector3 moveVec = {};

		if (collOperator.GetIsFindAttackTarget()) {
			// 攻撃対象が見つかっていれば、攻撃対象に向かって移動する
			moveVec = collOperator.GetTargetPos() - pos;

			// 攻撃対象に向かうように角度を変える
			angle.y = atan2f(moveVec.x, moveVec.z);
		}
		else {
			// 攻撃対象が見つかっていなければ、角度の方向に移動する
			moveVec = Vector3::XZonly(sinf(angle.y), cosf(angle.y));
		}

		// 最終的な移動ベクトルで移動する
		pos += moveVec.Normalized() * ATTACK_MOVE_SPEED;
	}
	else if (animePlayRate <= COLL_END_TIME[(int)attackStage]) {
		// 攻撃判定中
		collOperator.CollOn(attackStage);
	}
	else {
		// 攻撃判定終了後

		// 自身の遷移条件を呼び出し、キー入力で次段に繋がるようにしておく
		this->OwnStateConditionUpdate();

		// アニメーションが終了していたら入力関係なく指定の通常状態に遷移させる
		if (IsAnimeEnd()) { DefaultChangeState(); }
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
				attackStage = PLAYER_TRIPLE_ATTACK_STAGE::NON;
			}
		}

		// 次段に繋がるまでのカウンターを減らす
		if (attackNextStageContinueTimeCounter > 0) {
			attackNextStageContinueTimeCounter--;

			// 猶予時間がなくなったら
			if (attackNextStageContinueTimeCounter <= 0) {

				// 攻撃段数をリセットする
				attackStage = PLAYER_TRIPLE_ATTACK_STAGE::NON;

				// 攻撃のクールタイムをセットする
				coolTimeCounter = COOL_TIME;
			}
		}
	}

	// 探索情報をリセットする
	collOperator.ResetTarget();
}

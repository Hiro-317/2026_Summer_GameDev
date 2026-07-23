#include "GrapePlayerBombState.h"

#include "../../../../../../../Manager/Input/KeyManager.h"

GrapePlayerBombState::GrapePlayerBombState(
	const std::function<void(void)>& ownChangeState,
	const std::function<bool(void)>& isOwnState,
	GrapePlayerBombCollOperator& collOperator,
	const int COOL_TIME,
	const int ATTACK_COUNT_TIME,
	const int ATTACK_START_TIME,
	Vector3& pos,
	const std::function<void(void)> BombFallAnime,
	const std::function<bool(void)> IsAnimeEnd,
	const std::function<void(void)> DefaultChangeState
) :
	CharacterStateBase(ownChangeState, isOwnState),
	collOperator(collOperator),
	COOL_TIME(COOL_TIME),
	ATTACK_COUNT_TIME(ATTACK_COUNT_TIME),
	ATTACK_START_TIME(ATTACK_START_TIME),
	pos(pos),
	BombFallAnime(BombFallAnime),
	IsAnimeEnd(IsAnimeEnd),
	DefaultChangeState(DefaultChangeState),
	timeCounter(0),
	isInit(false)
{
}

void GrapePlayerBombState::OwnStateConditionUpdate(void)
{
	// 攻撃のクールタイム中は状態遷移しない
	if (coolTimeCounter > 0) { return; }

	// ステート遷移処理
	if (Key::GetIns().GetInfo(KEY_TYPE::PLAYER_SKILL_1).down) {
		OwnChangeState();
	}
}

void GrapePlayerBombState::Enter(void)
{
	// 
	isInit = true;
	
	// すべて初期化
	collOperator.CollOff();
	collOperator.ResetIsHit();
	collOperator.ResetIsTargetFind();
	collOperator.SetIsBombDraw(true);
	collOperator.SetIsEnemySerch(false);

	// 爆弾の設置場所を設定
	collOperator.SetPos();

	// クールタイムをセット
	coolTimeCounter = COOL_TIME;

	// タイマーを開始
	timeCounter = ATTACK_COUNT_TIME;

	// アニメーションを再生
	BombFallAnime();
}

void GrapePlayerBombState::Update(void)
{
	if (IsAnimeEnd()) {
		DefaultChangeState();
	}
}

void GrapePlayerBombState::AlwaysUpdate(void)
{
	// 自身の状態でないときは、攻撃のクールタイムを減らす
	if (!IsOwnState()) {
		// 攻撃のクールタイムを減らす
		if (coolTimeCounter > 0) { coolTimeCounter--; }
	}

#pragma region 設置後の処理
	if (collOperator.GetIsHit() || timeCounter == 0) { 
		return; 
	}

	// タイマースタート
	if (timeCounter > 1) { 
		timeCounter--; 
		collOperator.SetIsBombDraw(true);
	}

	// 設置してから二十秒後
	if (timeCounter < (ATTACK_COUNT_TIME - ATTACK_START_TIME)) {
		// 敵を探し始める
		collOperator.SetIsEnemySerch(true);
	}

	// タイマーが終了するか、爆弾の範囲内に敵が入ったら爆発する
	if (timeCounter == 1 || collOperator.GetIsAttackTargetFind()) {
		collOperator.CollOn();
		collOperator.SetIsBombDraw(false);
		timeCounter = 0;
	}

#pragma endregion 
}

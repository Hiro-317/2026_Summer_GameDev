#include "GrapePlayerBombState.h"

#include "../../../../../../../Manager/Input/KeyManager.h"

GrapePlayerBombState::GrapePlayerBombState(
	const std::function<void(void)>& ownChangeState,
	const std::function<bool(void)>& isOwnState,
	GrapePlayerBombCollOperator& collOperator,
	const int COOL_TIME,
	const float ATTACK_TIME,
	Vector3& pos,
	const std::function<void(void)> DefaultChangeState
) :
	CharacterStateBase(ownChangeState, isOwnState),
	collOperator(collOperator),
	COOL_TIME(COOL_TIME),
	ATTACK_TIME(ATTACK_TIME),
	pos(pos),
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
	isInit = true;

	// すべて初期化
	collOperator.CollOff();
	collOperator.ResetIsHit();

	// クールタイムをセット
	coolTimeCounter = COOL_TIME;

	// 爆弾の設置場所を設定
	collOperator.SetPos();

	// タイマーを開始
	timeCounter = 120;
	DefaultChangeState();
}

void GrapePlayerBombState::Update(void)
{


}

void GrapePlayerBombState::Exit(void)
{

}

void GrapePlayerBombState::AlwaysUpdate(void)
{
	if (timeCounter > 0) {
		timeCounter--;
	}
	else {
		if (isInit) {
			collOperator.CollOn();
			timeCounter = 0;
		}
	}

	if (collOperator.GetIsHit()) {
		collOperator.CollOff();
	}

	// 自身の状態でないときは、攻撃のクールタイムを減らす
	if (!IsOwnState()) {
		// 攻撃のクールタイムを減らす
		if (coolTimeCounter > 0) { coolTimeCounter--; }
	}
}

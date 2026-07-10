#include "PlayerSingleModifierState.h"

#include "../../../../../Manager/Sound/SoundManager.h"
#include "../../../../../Manager/Net/NetWorkManager.h"

#include "PlayerSingleModifierCollOperator.h"


PlayerSingleModifierState::PlayerSingleModifierState(
	const std::function<void(void)>& ownChangeState,
	const std::function<bool(void)>& isOwnState,

	PlayerSingleModifierCollOperator& collOperator,
	const std::vector<const Transform*>& targetTrans,

	float COLL_START_TIME, KEY_TYPE HEAL_KEY,
	int COOL_TIME,
	const std::function<void(void)> PlayHealAnime,
	const std::function<float(void)> GetAnimePlayRatio,
	const std::function<bool(void)> IsAnimeEnd,
	const std::function<void(void)> DefaultChangeState
) :
	CharacterStateBase(ownChangeState, isOwnState),


	collOperator(collOperator),
	targetTrans(targetTrans),
	COLL_START_TIME(COLL_START_TIME),
	HEAL_KEY(HEAL_KEY),
	COOL_TIME(COOL_TIME),
	PlayHealAnime(PlayHealAnime),
	GetAnimePlayRatio(GetAnimePlayRatio),
	IsAnimeEnd(IsAnimeEnd),
	DefaultChangeState(DefaultChangeState),
	isHealed(false)
{
}

void PlayerSingleModifierState::OwnStateConditionUpdate(void)
{
	// 攻撃のクールタイム中は状態遷移しない
	if (coolTimeCounter > 0) { return; }

	// 攻撃キーのダウントリガーで状態遷移
	if (Key::GetIns().GetInfo(HEAL_KEY).down) { OwnChangeState(); }
}

void PlayerSingleModifierState::Enter(void)
{
	coolTimeCounter = COOL_TIME;
	collOperator.CollOff();
	collOperator.ResetIsHit();
	isHealed = false;
	PlayHealAnime();

	targetIndex = 0;
}

void PlayerSingleModifierState::Update(void)
{
	if (!isHealed && GetAnimePlayRatio() > COLL_START_TIME) {
		collOperator.CollOn();
		collOperator.SetTargetPos(targetTrans.at(targetIndex)->pos);
		isHealed = true;
	}

	if (IsAnimeEnd()) { DefaultChangeState(); }
}

void PlayerSingleModifierState::Exit(void)
{
	collOperator.CollOff();
	collOperator.ResetIsHit();
	isHealed = false;
}

void PlayerSingleModifierState::AlwaysUpdate(void)
{
	// 自身の状態でないときは、攻撃のクールタイムを減らす
	if (!IsOwnState()) {
		// 攻撃のクールタイムを減らす
		if (coolTimeCounter > 0) { coolTimeCounter--; }
	}
}
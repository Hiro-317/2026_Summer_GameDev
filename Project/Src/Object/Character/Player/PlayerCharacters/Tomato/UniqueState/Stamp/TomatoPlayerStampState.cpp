#include "TomatoPlayerStampState.h"

#include "../../../../../../../Manager/Input/KeyManager.h"
#include "../../../../../../../Manager/Sound/SoundManager.h"
#include "../../../../../../../Manager/Camera/Camera.h"

TomatoPlayerStampState::TomatoPlayerStampState(
	const std::function<void(void)>& ownChangeState,
	const std::function<bool(void)>& isOwnState,
	//TomatoPlayerTackleCollOperator& collOperator,
	int COOL_TIME,
	float JUMP_POWER, 
	Vector3& pos, Vector3& angle,
	const std::function<void(void)> ChangeIsGravity,
	const std::function<bool(void)> IsGround,
	const std::function<void(void)> DefaultChangeState
) :
	CharacterStateBase(ownChangeState, isOwnState),
	//collOperator(collOperator),
	COOL_TIME(COOL_TIME),
	JUMP_POWER(JUMP_POWER),
	pos(pos), angle(angle),
	ChangeIsGravity(ChangeIsGravity),
	IsGround(IsGround),
	DefaultChangeState(DefaultChangeState),
	timeCounter(0)
{

}

void TomatoPlayerStampState::OwnStateConditionUpdate(void)
{
	// 攻撃のクールタイム中は状態遷移しない
	if (coolTimeCounter > 0) { return; }

	// ステート遷移処理
	if (Key::GetIns().GetInfo(KEY_TYPE::PLAYER_SKILL_3).down) {
		OwnChangeState();
	}
}

void TomatoPlayerStampState::Enter(void)
{
	coolTimeCounter = COOL_TIME;
	timeCounter = 420;

	ChangeIsGravity();

	angle.x = 0.0f;
}

void TomatoPlayerStampState::Update(void)
{
	timeCounter--;
	if (timeCounter >= 400) {
		pos.y += 30.0f;
	}

	if (timeCounter <= 350) {
		pos.y -= 60.0f;
		if (IsGround()) {
			DefaultChangeState();
		}
	}
}

void TomatoPlayerStampState::Exit(void)
{
	timeCounter = 0;
	ChangeIsGravity();
}

void TomatoPlayerStampState::AlwaysUpdate(void)
{
	// 自身の状態でないときは、攻撃のクールタイムを減らす
	if (!IsOwnState()) {
		// 攻撃のクールタイムを減らす
		if (coolTimeCounter > 0) { coolTimeCounter--; }
	}
}

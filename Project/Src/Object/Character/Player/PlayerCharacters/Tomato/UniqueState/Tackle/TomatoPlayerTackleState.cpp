#include "TomatoPlayerTackleState.h"

#include "../../../../../../../Manager/Input/KeyManager.h"

TomatoPlayerTackleState::TomatoPlayerTackleState(
	const std::function<void(void)>& ownChangeState,
	const std::function<bool(void)>& isOwnState,
	int COOL_TIME,
	float MOVE_SPEED, float ROTATION_POW,
	Vector3& pos, Vector3& angle,
	const std::function<void(void)> DefaultChangeState
	) : 
	CharacterStateBase(ownChangeState, isOwnState),
	COOL_TIME(COOL_TIME),
	MOVE_SPEED(MOVE_SPEED),
	ROTATION_POW(ROTATION_POW),
	pos(pos), angle(angle),
	DefaultChangeState(DefaultChangeState)
{

}

void TomatoPlayerTackleState::OwnStateConditionUpdate(void)
{
	// 攻撃のクールタイム中は状態遷移しない
	if (coolTimeCounter > 0) { return; }

	if (Key::GetIns().GetInfo(KEY_TYPE::PLAYER_SKILL_1).down) {
		OwnChangeState();
	}

}

void TomatoPlayerTackleState::Enter(void)
{
	coolTimeCounter = COOL_TIME;
}

void TomatoPlayerTackleState::Update(void)
{
	angle.x += Deg2Rad(40.0f);
	if (angle.x >= 120) {
		DefaultChangeState();
	}
}

void TomatoPlayerTackleState::Exit(void)
{
}

void TomatoPlayerTackleState::AlwaysUpdate(void)
{
	// 自身の状態でないときは、攻撃のクールタイムを減らす
	if (!IsOwnState()) {
		// 攻撃のクールタイムを減らす
		if (coolTimeCounter > 0) { coolTimeCounter--; }
	}
}

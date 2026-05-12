#include "TomatoMoveState.h"

TomatoMoveState::TomatoMoveState(
	const std::function<void(void)>& ownChangeState,
	const std::function<bool(void)>& isOwnState,
	float MOVE_SPEED, float ROTATION_POW,
	Vector3& pos, Vector3& angle, const Vector3& playerPos
	):CharacterStateBase(ownChangeState, isOwnState),
	MOVE_SPEED(MOVE_SPEED), ROTATION_POW(ROTATION_POW),
	pos(pos), angle(angle), playerPos(playerPos)
{
	
}

void TomatoMoveState::Enter(void)
{
}

void TomatoMoveState::Update(void)
{
	// 回転の更新
	moveDir = (playerPos - pos).Normalized();
	angle.y = atan2f(moveDir.x, moveDir.z);
	angle.x += ROTATION_POW;

	// 位置の更新
	pos += moveDir * MOVE_SPEED;
}

void TomatoMoveState::Exit(void)
{
}

void TomatoMoveState::AlwaysUpdate(void)
{
}

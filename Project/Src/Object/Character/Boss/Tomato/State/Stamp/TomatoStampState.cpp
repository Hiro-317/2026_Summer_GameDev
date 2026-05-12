#include "TomatoStampState.h"

TomatoStampState::TomatoStampState(
	const std::function<void(void)>& ownChangeState,
	const std::function<bool(void)>& isOwnState,
	float MOVE_SPEED, float ATTACK_RADIUS,
	Vector3& pos, const Vector3& playerPos
) :CharacterStateBase(ownChangeState, isOwnState),
	MOVE_SPEED(MOVE_SPEED), ATTACK_RADIUS(ATTACK_RADIUS),
	pos(pos), playerPos(playerPos)
{
}

void TomatoStampState::Enter(void)
{
	attackPos = playerPos;
	prevPos = pos.y;
	pos.y += JUMP_POW;
}

void TomatoStampState::Update(void)
{
	if (prevPos <= pos.y) {
		pos.y += JUMP_POW;
		
		prevPos = pos.y;
	}
}

void TomatoStampState::Exit(void)
{
}

void TomatoStampState::AlwaysUpdate(void)
{
}

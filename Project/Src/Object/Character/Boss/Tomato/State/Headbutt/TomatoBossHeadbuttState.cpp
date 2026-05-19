#include "TomatoBossHeadbuttState.h"

TomatoBossHeadbuttState::TomatoBossHeadbuttState(
	const std::function<void(void)>& ownChangeState,
	const std::function<bool(void)>& isOwnState,
	const float MOVE_SPEED, const float ATTACK_TIME,
	Vector3& pos, Vector3& angle, const Vector3& playerPos,
	const std::function<void(void)> DefaultChangeState,
	const std::function<void(void)> resetAngle
) 
	:CharacterStateBase(ownChangeState, isOwnState),
	MOVE_SPEED(MOVE_SPEED), ATTACK_TIME(ATTACK_TIME),
	pos(pos), angle(angle), playerPos(playerPos),
	DefaultChangeState(DefaultChangeState),
	resetAngle(resetAngle)
{
}

void TomatoBossHeadbuttState::Enter(void)
{
	moveDir = (playerPos - pos).Normalized();
	resetAngle();
	time = 0;
}

void TomatoBossHeadbuttState::Update(void)
{
	time++;
	if (time > ATTACK_TIME) {
		Exit();
		DefaultChangeState();
	}
	else {
		// à íuÇÃçXêV
		float cnt = time <= ATTACK_TIME / 2.0f ? time : time - (ATTACK_TIME + 1.0f);
		pos += moveDir * MOVE_SPEED * cnt;
	}
}

void TomatoBossHeadbuttState::Exit(void)
{
}

void TomatoBossHeadbuttState::AlwaysUpdate(void)
{
}

#include "TomatoBossTackleState.h"

TomatoBossTackleState::TomatoBossTackleState(
	const std::function<void(void)>& ownChangeState,
	const std::function<bool(void)>& isOwnState,
	float MOVE_SPEED, float ROTATION_POW,
	Vector3& pos, Vector3& angle, const Vector3& playerPos
) :CharacterStateBase(ownChangeState, isOwnState),
MOVE_SPEED(MOVE_SPEED), ROTATION_POW(ROTATION_POW),
pos(pos), angle(angle), playerPos(playerPos)
{
	rotPow = ROTATION_POW;
}

void TomatoBossTackleState::Enter(void)
{
	time = 0;
}

void TomatoBossTackleState::Update(void)
{
	// 一秒の溜め
	if (time < 180) {
		
		time++;

		// 回転の更新
		moveDir = (playerPos - pos).Normalized();
		angle.y = atan2f(moveDir.x, moveDir.z);
		rotPow += ROTATION_POW;
	}
	else {
		// 位置の更新
		pos += moveDir * MOVE_SPEED;
	}
	angle.x += rotPow;
}

void TomatoBossTackleState::Exit(void)
{
}

void TomatoBossTackleState::AlwaysUpdate(void)
{
}

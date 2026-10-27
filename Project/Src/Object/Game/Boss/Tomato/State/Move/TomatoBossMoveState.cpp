#include "TomatoBossMoveState.h"

#include "../../../../../../Manager/Net/NetWorkManager.h"

TomatoBossMoveState::TomatoBossMoveState(
	float MOVE_SPEED, float ROTATION_POW,
	Vector3& pos, Quaternion& rotation, const Vector3* const (&playerPos)[(int)MSG_SENDER_ID::Max],
	const std::function<int(void)> GetTarget,
	const std::function<void(void)> ResetAngle,
	const std::function<void(void)> HeadbuttChangeState
)
	:CharacterStateBase(),
	MOVE_SPEED(MOVE_SPEED), ROTATION_POW(ROTATION_POW),
	pos(pos), rotation(rotation), playerPos(playerPos),
	GetTarget(GetTarget),
	ResetAngle(ResetAngle),
	HeadbuttChangeState(HeadbuttChangeState)
{
	
}

void TomatoBossMoveState::Enter(void)
{
	target = GetTarget();
}

void TomatoBossMoveState::Update(void)
{
	// 回転の更新
	moveDir = (*playerPos[target] - pos).Normalized();
	rotation = Quaternion::FromRotationY(atan2f(moveDir.x, moveDir.z));
	rotation.AddAngle(Vector3(ROTATION_POW, 0.0f, 0.0f));

	// 位置の更新
	pos += moveDir * MOVE_SPEED;

	if ((*playerPos[target] - pos).Length() <= 350.0f) {
		HeadbuttChangeState();
	}
}

void TomatoBossMoveState::Exit(void)
{
	ResetAngle();
}

void TomatoBossMoveState::AlwaysUpdate(void)
{
}

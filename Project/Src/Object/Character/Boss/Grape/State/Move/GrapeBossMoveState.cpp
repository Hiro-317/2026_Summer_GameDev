#include "GrapeBossMoveState.h"

#include "../../../../../../Manager/Net/NetWorkManager.h"

GrapeBossMoveState::GrapeBossMoveState(
	const std::function<void(void)>& ownChangeState,
	const std::function<bool(void)>& isOwnState,
	float MOVE_SPEED,
	Vector3& pos, Vector3& angle, const std::vector<const Vector3*> playerPos,
	const std::function<int(void)> GetTarget,
	const std::function<void(void)> ResetAngle,
	const std::function<void(void)> headbuttChangeState
)
	:CharacterStateBase(ownChangeState, isOwnState),
	MOVE_SPEED(MOVE_SPEED),
	pos(pos), angle(angle), playerPos(playerPos),
	GetTarget(GetTarget),
	ResetAngle(ResetAngle),
	headbuttChangeState(headbuttChangeState)
{
	
}

void GrapeBossMoveState::Enter(void)
{
	target = GetTarget();
}

void GrapeBossMoveState::Update(void)
{
	// 回転の更新
	moveDir = (*playerPos.at(target) - pos).Normalized();
	angle.y = atan2f(moveDir.x, moveDir.z);

	// 位置の更新
	pos += moveDir * MOVE_SPEED;

	if ((*playerPos.at(target) - pos).Length() <= 350.0f) {
		headbuttChangeState();
	}
}

void GrapeBossMoveState::Exit(void)
{
}

void GrapeBossMoveState::AlwaysUpdate(void)
{
}

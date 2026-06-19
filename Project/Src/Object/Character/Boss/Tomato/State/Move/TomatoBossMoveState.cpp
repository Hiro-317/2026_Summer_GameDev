#include "TomatoBossMoveState.h"

#include "../../../../../../Manager/Net/NetWorkManager.h"

TomatoBossMoveState::TomatoBossMoveState(
	const std::function<void(void)>& ownChangeState,
	const std::function<bool(void)>& isOwnState,
	float MOVE_SPEED, float ROTATION_POW,
	Vector3& pos, Vector3& angle, const std::vector<const Vector3*> playerPos,
	const std::function<int(void)> GetTarget,
	const std::function<void(void)> ResetAngle,
	const std::function<void(void)> headbuttChangeState
)
	:CharacterStateBase(ownChangeState, isOwnState),
	MOVE_SPEED(MOVE_SPEED), ROTATION_POW(ROTATION_POW),
	pos(pos), angle(angle), playerPos(playerPos),
	GetTarget(GetTarget),
	ResetAngle(ResetAngle),
	headbuttChangeState(headbuttChangeState)
{
	
}

void TomatoBossMoveState::Enter(void)
{
	if(Net::GetIns().IsHost()){
		Net::GetIns().Send(MsgDataBossInform(MsgDataBossInform::INFORM_TYPE::ChangeMove));
	}
}

void TomatoBossMoveState::Update(void)
{
	// 回転の更新
	moveDir = (*playerPos.at(GetTarget()) - pos).Normalized();
	angle.y = atan2f(moveDir.x, moveDir.z);
	angle.x += ROTATION_POW;

	// 位置の更新
	pos += moveDir * MOVE_SPEED;

	if ((*playerPos.at(GetTarget()) - pos).Length() <= 350.0f) {
		headbuttChangeState();
	}
}

void TomatoBossMoveState::Exit(void)
{
	ResetAngle();
}

void TomatoBossMoveState::AlwaysUpdate(void)
{
}

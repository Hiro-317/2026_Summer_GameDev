#include "GrapeBossMoveState.h"

#include "../../../../../../Manager/Net/NetWorkManager.h"

#include "../../../../../Common/Transform/Transform.h"

GrapeBossMoveState::GrapeBossMoveState(
	float MOVE_SPEED,
	Transform& bossTrans,
	const Vector3* const (&playerPos)[(int)MSG_SENDER_ID::Max],
	const std::function<int(void)> GetTarget,
	const std::function<void(void)> PlayRunAnime,
	const std::function<void(void)> KickDownChangeState
) :
	MOVE_SPEED(MOVE_SPEED),
	bossTrans(bossTrans),
	playerPos(playerPos),
	GetTarget(GetTarget),
	PlayRunAnime(PlayRunAnime),
	KickDownChangeState(KickDownChangeState)
{
	
}

void GrapeBossMoveState::Enter(void)
{
	target = GetTarget();
	PlayRunAnime();
}

void GrapeBossMoveState::Update(void)
{
	// 回転の更新
	moveDir = (*playerPos[target] - bossTrans.pos).Normalized();
	bossTrans.SetAngle(Vector3::Yonly(atan2f(moveDir.x, moveDir.z)));

	// 位置の更新
	bossTrans.pos += moveDir * MOVE_SPEED;

	if (((*playerPos[target]) - bossTrans.pos).Length() <= 450.0f) {
		KickDownChangeState();
	}
}

void GrapeBossMoveState::Exit(void)
{
}

void GrapeBossMoveState::AlwaysUpdate(void)
{
}

#include "GrapeBossMoveState.h"

#include "../../../../../../Manager/Net/NetWorkManager.h"

#include "../../../../../Common/Transform/Transform.h"

GrapeBossMoveState::GrapeBossMoveState(
	float MOVE_SPEED,

	const Vector3& bossPos,

	const Vector3* const (&playerPos)[(int)MSG_SENDER_ID::Max],
	const int& targetNum,

	const std::function<void(const Vector3&)>& moveAccel,

	const std::function<void(void)>& playRunAnime,
	const std::function<void(void)>& kickDownChangeState
) :
	MOVE_SPEED(MOVE_SPEED),

	bossPos(bossPos),

	playerPos(playerPos),
	targetNum(targetNum),

	moveAccel(moveAccel),

	playRunAnime(playRunAnime),
	kickDownChangeState(kickDownChangeState)
{
	
}

void GrapeBossMoveState::Enter(void)
{
	playRunAnime();
}

void GrapeBossMoveState::Update(void)
{
	Vector3 vec = *playerPos[targetNum] - bossPos;

	if (vec.Length() <= 450.0f) { kickDownChangeState(); }

	moveAccel(vec.Normalized());
}

void GrapeBossMoveState::Exit(void)
{
}

void GrapeBossMoveState::AlwaysUpdate(void)
{
}

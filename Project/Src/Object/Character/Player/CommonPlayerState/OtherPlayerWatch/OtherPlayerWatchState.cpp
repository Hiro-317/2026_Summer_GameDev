#include "OtherPlayerWatchState.h"

#include "../../../../../Manager/Input/KeyManager.h"

OtherPlayerWatchState::OtherPlayerWatchState(
	const std::function<void(void)>& ownChangeState,
	const std::function<bool(void)>& isOwnState, 
	const std::vector<const Vector3*> pos
) :
	CharacterStateBase(ownChangeState,isOwnState),
	pos(pos)
{
}

void OtherPlayerWatchState::Enter(void)
{
	Camera::GetIns().FollowAutoToLookTargetChange(pos.front());
}

void OtherPlayerWatchState::Update(void)
{

}

void OtherPlayerWatchState::Exit(void)
{
}

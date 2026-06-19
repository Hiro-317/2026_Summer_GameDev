#include "OtherPlayerWatchState.h"

#include "../../../../../Manager/Camera/Camera.h"

#include "../../../../../Manager/Input/KeyManager.h"


OtherPlayerWatchState::OtherPlayerWatchState(
	const std::function<void(void)>& ownChangeState,
	const std::function<bool(void)>& isOwnState, 
	const std::vector<const Transform*>& playerTrans,
	const Vector3*& bossPos
) :
	CharacterStateBase(ownChangeState,isOwnState),
	playerTrans(playerTrans),
	bossPos(bossPos)
{
}

void OtherPlayerWatchState::Enter(void)
{
	Camera::GetIns().ChangeModeFollowAuto(*playerTrans.front(), bossPos);
}

void OtherPlayerWatchState::Update(void)
{

}

void OtherPlayerWatchState::Exit(void)
{
}

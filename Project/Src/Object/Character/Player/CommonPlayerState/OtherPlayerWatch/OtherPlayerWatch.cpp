#include "OtherPlayerWatch.h"

#include "../../../../../Manager/Input/KeyManager.h"

OtherPlayerWatch::OtherPlayerWatch(
	const std::function<void(void)>& ownChangeState,
	const std::function<bool(void)>& isOwnState, 
	std::vector<Vector3>& pos,
	const std::function<void(void)> CameraTargetChange
) :
	CharacterStateBase(ownChangeState,isOwnState),
	pos(pos),
	CameraTargetChange(CameraTargetChange)
{
}

void OtherPlayerWatch::Enter(void)
{
	
}

void OtherPlayerWatch::Update(void)
{
}

void OtherPlayerWatch::Exit(void)
{
}

#include "OtherPlayerWatchState.h"

#include "../../../../../Manager/Net/NetWorkManager.h"

#include "../../../../../Manager/Camera/CurrentCamera.h"
#include "../../../../../Manager/Camera/MultifuncCamera/MultifuncCamera.h"

#include "../../../../../Manager/Input/InputManager.h"

OtherPlayerWatchState::OtherPlayerWatchState(const std::vector<const Transform*>& playerTrans, const Vector3*& bossPos) :
	playerTrans(playerTrans),
	bossPos(bossPos)
{
}

void OtherPlayerWatchState::Enter(void)
{
	dynamic_cast<MultifuncCamera*>(&CurrentCamera::Get())->ChangeModeFollowAuto(&playerTrans.front()->pos, bossPos);
}

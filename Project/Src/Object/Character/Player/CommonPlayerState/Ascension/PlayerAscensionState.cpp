#include "PlayerAscensionState.h"

PlayerAscensionState::PlayerAscensionState(
	const std::function<void(void)>& ownChangeState,
	const std::function<bool(void)>& isOwnState,
	const std::function<void()> DefaultChangeState
) :
	CharacterStateBase(ownChangeState, isOwnState),
	DefaultChangeState(DefaultChangeState)
{
}

void PlayerAscensionState::Enter(void)
{
}

void PlayerAscensionState::Update(void)
{
}

void PlayerAscensionState::Exit(void)
{
}


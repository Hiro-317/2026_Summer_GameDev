#include "PlayerAscensionState.h"

PlayerAscensionState::PlayerAscensionState(
	const std::function<void(void)>& ownChangeState,
	const std::function<bool(void)>& isOwnState,
	const std::function<bool(void)>& isPrevention,
	PlayerAscensionCollOperator& collOperator,
	const std::function<void()> DefaultChangeState
) :
	CharacterStateBase(ownChangeState, isOwnState),
	collOperator(collOperator),
	DefaultChangeState(DefaultChangeState),
	isPrevention(isPrevention)
{
}

void PlayerAscensionState::Enter(void)
{
	collOperator.CollOn();

	aliveCounter = ALIVE_TIME;
	charge = 0;
}

void PlayerAscensionState::Update(void)
{
	if (isPrevention) {

	}
}

void PlayerAscensionState::Exit(void)
{
	collOperator.CollOff();
}


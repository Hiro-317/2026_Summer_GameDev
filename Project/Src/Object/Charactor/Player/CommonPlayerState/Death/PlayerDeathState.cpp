#include "PlayerDeathState.h"

PlayerDeathState::PlayerDeathState(
	const std::function<void(void)>& ownChangeState, 
	const std::function<bool(void)>& isOwnState, 
	const std::function<void(void)> PlayDeathAnime, 
	const std::function<void(void)> DefaultChangeState
):
	CharactorStateBase(ownChangeState,isOwnState),
	PlayDeathAnime(PlayDeathAnime),
	DefaultChangeState(DefaultChangeState)
{
}

void PlayerDeathState::Enter(void)
{
	PlayDeathAnime();
}

void PlayerDeathState::Update(void)
{
	if (false) {
		DefaultChangeState();
	}
}

void PlayerDeathState::Exit(void)
{
}

#include "PlayerDeathState.h"

PlayerDeathState::PlayerDeathState(
	const std::function<void(void)>& ownChangeState, 
	const std::function<bool(void)>& isOwnState, 
	Vector3& pos, Vector3& angle,
	const std::function<bool(void)> IsAnimeEnd,
	const std::function<void(void)> PlayDeathAnime,
	const std::function<void(void)> DeathCamera,
	const std::function<void(void)> ChangeGameOver,
	const std::function<void(void)> DefaultChangeState
):
	CharacterStateBase(ownChangeState,isOwnState),
	pos(pos), angle(angle),
	IsAnimeEnd(IsAnimeEnd),
	PlayDeathAnime(PlayDeathAnime),
	DeathCamera(DeathCamera),
	ChangeGameOver(ChangeGameOver)
{
}

void PlayerDeathState::Enter(void)
{
	PlayDeathAnime();
	DeathCamera();
}

void PlayerDeathState::Update(void)
{
	if (IsAnimeEnd()) {
		DefaultChangeState();
	}
}

void PlayerDeathState::Exit(void)
{
}

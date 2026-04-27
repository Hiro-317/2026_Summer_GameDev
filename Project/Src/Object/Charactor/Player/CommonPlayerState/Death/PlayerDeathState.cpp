#include "PlayerDeathState.h"

PlayerDeathState::PlayerDeathState(
	const std::function<void(void)>& ownChangeState, 
	const std::function<bool(void)>& isOwnState, 
	Vector3& pos, Vector3& angle,
	const std::function<bool(void)> IsAnimeEnd,
	const std::function<void(void)> PlayDeathAnime, 
	const std::function<void(void)> DefaultChangeState
):
	CharactorStateBase(ownChangeState,isOwnState),
	pos(pos), angle(angle),
	IsAnimeEnd(IsAnimeEnd),
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
	if (IsAnimeEnd()) {
		pos += Vector3::Yonly(1);

	}
}

void PlayerDeathState::Exit(void)
{
}

#include "PlayerDeathState.h"

PlayerDeathState::PlayerDeathState(
	Vector3& pos, Quaternion& rotation,
	const std::function<bool(void)> IsAnimeEnd,
	const std::function<void(void)> PlayDeathAnime,
	const std::function<void(void)> PlayerDeathSetting,
	const std::function<void(void)> DefaultChangeState
):
	pos(pos), rotation(rotation),
	IsAnimeEnd(IsAnimeEnd),
	PlayDeathAnime(PlayDeathAnime),
	PlayerDeathSetting(PlayerDeathSetting),
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
		DefaultChangeState();
	}
}

void PlayerDeathState::Exit(void)
{
	PlayerDeathSetting();
}

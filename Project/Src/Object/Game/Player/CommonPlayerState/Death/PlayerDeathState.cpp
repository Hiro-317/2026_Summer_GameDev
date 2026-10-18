#include "PlayerDeathState.h"

PlayerDeathState::PlayerDeathState(
	Vector3& pos, Quaternion& rotation,
	std::function<bool(void)> isAnimeEnd,
	std::function<void(void)> playDeathAnime,
	std::function<void(void)> playerDeathSetting,
	std::function<void(void)> defaultChangeState
):
	pos(pos), rotation(rotation),
	isAnimeEnd(isAnimeEnd),
	playDeathAnime(playDeathAnime),
	playerDeathSetting(playerDeathSetting),
	defaultChangeState(defaultChangeState)
{
}

void PlayerDeathState::Enter(void)
{
	playDeathAnime();
}

void PlayerDeathState::Update(void)
{
	if (isAnimeEnd()) { defaultChangeState(); }
}

void PlayerDeathState::Exit(void)
{
	playerDeathSetting();
}

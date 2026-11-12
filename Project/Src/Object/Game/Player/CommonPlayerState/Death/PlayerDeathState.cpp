#include "PlayerDeathState.h"

PlayerDeathState::PlayerDeathState(
	std::function<void(void)> playAnimeDeath,
	std::function<bool(void)> isAnimeEnd,
	std::function<void(void)> playerDeathSetting
):
	playAnimeDeath(playAnimeDeath),
	isAnimeEnd(isAnimeEnd),
	playerDeathSetting(playerDeathSetting)
{
}

void PlayerDeathState::Enter(void)
{
	playAnimeDeath();
}

void PlayerDeathState::Update(void)
{
	if (isAnimeEnd()) { playerDeathSetting(); }
}

void PlayerDeathState::Exit(void)
{
}

#include "TomatoPlayerDeathState.h"

#include "../../../../../../../Utility/Utility.h"

#include "../../../../../../../Manager/TimeScale/TimeScale.h"

#include "../../../../../../Common/Transform/Transform.h"

TomatoPlayerDeathState::TomatoPlayerDeathState(Transform& playerTrans, std::function<void(void)> playerDeathSetting) :
	playerTrans(playerTrans),
	playerDeathSetting(playerDeathSetting),

	animeCounter(0.0f)
{
}

void TomatoPlayerDeathState::Enter(void)
{
	animeCounter = 30.0f;
	playerTrans.rotation = Quaternion::FromRotationX(Deg2Rad(180.0f));
}

void TomatoPlayerDeathState::Update(void)
{
	playerTrans.pos.y += animeCounter*TimeScale::Get();
	animeCounter -= 0.2f*TimeScale::Get();
	playerTrans.AddAngleYDeg(3.0f * TimeScale::Get());

	if (animeCounter <= 0.0f) {
		playerDeathSetting();
		animeCounter = 0.0f;
	}
}

void TomatoPlayerDeathState::Exit(void)
{
	animeCounter = 0.0f;
}

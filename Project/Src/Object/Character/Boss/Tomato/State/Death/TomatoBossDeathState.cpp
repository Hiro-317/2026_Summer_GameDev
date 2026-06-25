#include "TomatoBossDeathState.h"

#include "../../../../../../Scene/Game/GameScene.h"


TomatoBossDeathState::TomatoBossDeathState(
	const std::function<void(void)>& ownChangeState,
	const std::function<bool(void)>& isOwnState,
	Vector3& scale, const Vector3& defaultScale,
	const std::function<void(void)> IsDeath
)
	:CharacterStateBase(ownChangeState, isOwnState),
	scale(scale), defaultScale(defaultScale),
	IsDeath(IsDeath)
{
}

void TomatoBossDeathState::Enter(void)
{
	GameScene::Shake(ShakeKinds::DIAG, ShakeSize::BIG, CHANGE_COUNT);
}

void TomatoBossDeathState::Update(void)
{
	if (cnt <= CHANGE_COUNT) {

		float rate = ((float)cnt - SCALE_REVERSE) * ((float)cnt - SCALE_REVERSE);
		scale = defaultScale * (rate * BOMB_RATE);
		cnt++;
	}
	else {
		IsDeath();
	}
}

void TomatoBossDeathState::Exit(void)
{
}

void TomatoBossDeathState::AlwaysUpdate(void)
{
}

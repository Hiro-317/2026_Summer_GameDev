#include "GrapeBossDeathState.h"

#include "../../../../../../Scene/Game/GameScene.h"


GrapeBossDeathState::GrapeBossDeathState(
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

void GrapeBossDeathState::Enter(void)
{
	GameScene::Shake(ShakeKinds::DIAG, ShakeSize::BIG, CHANGE_COUNT);
}

void GrapeBossDeathState::Update(void)
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

void GrapeBossDeathState::Exit(void)
{
}

void GrapeBossDeathState::AlwaysUpdate(void)
{
}

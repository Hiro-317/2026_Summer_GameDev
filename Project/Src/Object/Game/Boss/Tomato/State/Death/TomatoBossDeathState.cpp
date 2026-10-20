#include "TomatoBossDeathState.h"

#include "../../../../../../Scene/Game/GameScene.h"
#include "../../../../../../Manager/Net/NetWorkManager.h"


TomatoBossDeathState::TomatoBossDeathState(
	const std::function<void(void)>& ownChangeState,
	const std::function<bool(void)>& isOwnState,
	Vector3& scale, Quaternion& rotation,
	const std::function<void(void)> IsDeath
)
	:CharacterStateBase(ownChangeState, isOwnState),
	scale(scale), rotation(rotation), defaultScale(defaultScale),
	IsDeath(IsDeath)
{
}

void TomatoBossDeathState::Enter(void)
{
	GameScene::Shake(ShakeKinds::DIAG, ShakeSize::BIG, CHANGE_COUNT);
}

void TomatoBossDeathState::Update(void)
{
	// âÒì]ÇµÇ»Ç™ÇÁè¨Ç≥Ç≠Ç≥ÇπÇÈ
	if (cnt <= CHANGE_COUNT) {

		cnt++;
		rotation.AddAngle(Vector3(0.0f, ROTATION_POW, 0.0f));
		scale *= SCALE_POW;
	}
	else {
		Net::GetIns().Send(MsgDataBossIsDeath(true));
		IsDeath();
	}
}

void TomatoBossDeathState::Exit(void)
{
}

void TomatoBossDeathState::AlwaysUpdate(void)
{
}

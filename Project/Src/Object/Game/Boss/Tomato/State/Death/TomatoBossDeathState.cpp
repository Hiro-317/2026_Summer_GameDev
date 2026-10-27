#include "TomatoBossDeathState.h"

#include "../../../../../../Scene/SceneBase.h"
#include "../../../../../../Scene/SceneManager.h"
#include "../../../../../../Scene/Common/PostEffect/ScreenShakePostEffect/ScreenShakePostEffect.h"

#include "../../../../../../Manager/Net/NetWorkManager.h"


TomatoBossDeathState::TomatoBossDeathState(
	Vector3& scale, Quaternion& rotation,
	const std::function<void(void)> IsDeath
)
	:CharacterStateBase(),
	scale(scale), rotation(rotation), defaultScale(defaultScale),
	IsDeath(IsDeath)
{
}

void TomatoBossDeathState::Enter(void)
{
	SceneManager::GetIns().GetScene()->AddPostEffect(new ScreenShakePostEffect());
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

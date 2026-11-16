#include "TomatoBossDeathState.h"

#include "../../../../../../Scene/SceneBase.h"
#include "../../../../../../Scene/SceneManager.h"
#include "../../../../../../Scene/Common/PostEffect/ScreenShakePostEffect/ScreenShakePostEffect.h"

#include "../../../../../../Manager/Net/NetWorkManager.h"


TomatoBossDeathState::TomatoBossDeathState(
	Vector3& scale, Quaternion& rotation,
	const std::function<void(void)> MarkDead
)
	:CharacterStateBase(),
	scale(scale), rotation(rotation),
	MarkDead(MarkDead)
{
}

void TomatoBossDeathState::Enter(void)
{
	elapsedFrames = 0;
	hasNotifiedDeath = false;
	SceneManager::GetIns().GetScene()->AddPostEffect(new ScreenShakePostEffect(0.008f, 20.0f, 120.0f));
}

void TomatoBossDeathState::Update(void)
{
	if (hasNotifiedDeath) { return; }
	// âÒì]ÇµÇ»Ç™ÇÁè¨Ç≥Ç≠Ç≥ÇπÇÈ
	if (elapsedFrames <= CHANGE_COUNT) {

		elapsedFrames++;
		rotation.AddAngle(Vector3(0.0f, ROTATION_POW, 0.0f));
		scale *= SCALE_POW;
	}
	else {
		hasNotifiedDeath = true;
		Net::GetIns().Send(MsgDataBossIsDeath(true));
		MarkDead();
	}
}

void TomatoBossDeathState::Exit(void)
{
}

void TomatoBossDeathState::AlwaysUpdate(void)
{
}

#include "BananaBossDeathState.h"

#include "../../../../../../Scene/SceneManager.h"
#include "../../../../../../Scene/SceneBase.h"
#include "../../../../../../Scene/Common/PostEffect/ScreenShakePostEffect/ScreenShakePostEffect.h"

#include "../../../../../../Manager/Net/NetWorkManager.h"


BananaBossDeathState::BananaBossDeathState(
	const std::function<void(void)>& PlayDeath,
	const std::function<void(void)> MarkDead
)
	:CharacterStateBase(),
	PlayDeath(PlayDeath),
	MarkDead(MarkDead)
{
}

void BananaBossDeathState::Enter(void)
{
	elapsedFrames = 0;
	hasNotifiedDeath = false;
	SceneManager::GetIns().GetScene()->AddPostEffect(new ScreenShakePostEffect(0.008f, 20.0f, 120.0f));
	PlayDeath();
}

void BananaBossDeathState::Update(void)
{
	if (hasNotifiedDeath) { return; }
	// アニメーションが終わっていたらシーン移行
	if (elapsedFrames >= CHANGE_COUNT) {
		hasNotifiedDeath = true;
		Net::GetIns().Send(MsgDataBossIsDeath(true));
		MarkDead();
	}
	// チェンジ用のカウント
	elapsedFrames++;
}

void BananaBossDeathState::Exit(void)
{
}

void BananaBossDeathState::AlwaysUpdate(void)
{
}

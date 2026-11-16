#include "GrapeBossDeathState.h"

#include "../../../../../../Manager/Net/NetWorkManager.h"

#include "../../../../../../Scene/SceneManager.h"
#include "../../../../../../Scene/SceneBase.h"

#include "../../../../../../Scene/Common/PostEffect/ScreenShakePostEffect/ScreenShakePostEffect.h"


GrapeBossDeathState::GrapeBossDeathState(
	const std::function<void(void)> DeathAnim,
	const std::function<float(void)> AnimeTotalTime,
	const std::function<bool(void)> IsAnimEnd,
	const std::function<void(void)> MarkDead
):
	DeathAnim(DeathAnim), AnimeTotalTime(AnimeTotalTime),
	IsAnimEnd(IsAnimEnd), MarkDead(MarkDead)
{
}

void GrapeBossDeathState::Enter(void)
{
	elapsedFrames = 0;
	hasNotifiedDeath = false;
	DeathAnim();
	deathDelayFrames = ((int)AnimeTotalTime() + DEATH_LAST_TIME);
	SceneManager::GetIns().GetScene()->AddPostEffect(new ScreenShakePostEffect(0.008f, 20.0f, 120.0f));
	elapsedFrames = 0;
}

void GrapeBossDeathState::Update(void)
{
	if (hasNotifiedDeath) { return; }
	// アニメーションが終わっていたらシーン移行
	if (elapsedFrames >= deathDelayFrames) {
		hasNotifiedDeath = true;
		Net::GetIns().Send(MsgDataBossIsDeath(true));
		MarkDead();
	}
	// チェンジ用のカウント
	elapsedFrames++;
}

void GrapeBossDeathState::Exit(void)
{
}

void GrapeBossDeathState::AlwaysUpdate(void)
{
}

#include "GrapeBossDeathState.h"

#include "../../../../../../Manager/Net/NetWorkManager.h"

#include "../../../../../../Scene/SceneManager.h"
#include "../../../../../../Scene/SceneBase.h"

#include "../../../../../../Scene/Common/PostEffect/ScreenShakePostEffect/ScreenShakePostEffect.h"


GrapeBossDeathState::GrapeBossDeathState(
	const std::function<void(void)> DeathAnim,
	const std::function<float(void)> AnimeTotalTime,
	const std::function<bool(void)> IsAnimEnd,
	const std::function<void(void)> IsDeath
):
	DeathAnim(DeathAnim), AnimeTotalTime(AnimeTotalTime),
	IsAnimEnd(IsAnimEnd), IsDeath(IsDeath)
{
}

void GrapeBossDeathState::Enter(void)
{
	DeathAnim();
	changeTime = ((int)AnimeTotalTime() + DEATH_LAST_TIME);
	SceneManager::GetIns().GetScene()->AddPostEffect(new ScreenShakePostEffect());
	cnt = 0;
}

void GrapeBossDeathState::Update(void)
{
	// アニメーションが終わっていたらシーン移行
	if (cnt >= changeTime) {
		Net::GetIns().Send(MsgDataBossIsDeath(true));
		IsDeath();
	}
	// チェンジ用のカウント
	cnt++;
}

void GrapeBossDeathState::Exit(void)
{
}

void GrapeBossDeathState::AlwaysUpdate(void)
{
}

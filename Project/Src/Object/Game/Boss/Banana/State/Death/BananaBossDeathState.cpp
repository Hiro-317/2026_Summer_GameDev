#include "BananaBossDeathState.h"

#include "../../../../../../Scene/SceneManager.h"
#include "../../../../../../Scene/SceneBase.h"
#include "../../../../../../Scene/Common/PostEffect/ScreenShakePostEffect/ScreenShakePostEffect.h"

#include "../../../../../../Manager/Net/NetWorkManager.h"


BananaBossDeathState::BananaBossDeathState(
	const std::function<void(void)>& PlayDeath,
	const std::function<void(void)> IsDeath
)
	:CharacterStateBase(),
	PlayDeath(PlayDeath),
	IsDeath(IsDeath)
{
}

void BananaBossDeathState::Enter(void)
{
	SceneManager::GetIns().GetScene()->AddPostEffect(new ScreenShakePostEffect());
	PlayDeath();
}

void BananaBossDeathState::Update(void)
{
	// アニメーションが終わっていたらシーン移行
	if (cnt >= CHANGE_COUNT) {
		Net::GetIns().Send(MsgDataBossIsDeath(true));
		IsDeath();
	}
	// チェンジ用のカウント
	cnt++;
}

void BananaBossDeathState::Exit(void)
{
}

void BananaBossDeathState::AlwaysUpdate(void)
{
}

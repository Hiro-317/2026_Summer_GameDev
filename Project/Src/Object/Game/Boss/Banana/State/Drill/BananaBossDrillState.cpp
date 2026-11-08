#include "BananaBossDrillState.h"

#include "../../../../../../Scene/SceneManager.h"
#include "../../../../../../Scene/SceneBase.h"

#include "../../../../../../Manager/Net/NetWorkManager.h"


BananaBossDrillState::BananaBossDrillState(
	const std::function<void(void)>& PlayDeath,
	const std::function<void(void)> IsDeath
)
	:CharacterStateBase(),
	PlayDeath(PlayDeath),
	IsDeath(IsDeath)
{
}

void BananaBossDrillState::Enter(void)
{
	PlayDeath();
}

void BananaBossDrillState::Update(void)
{
	// アニメーションが終わっていたらシーン移行
	if (cnt >= CHANGE_COUNT) {
		Net::GetIns().Send(MsgDataBossIsDeath(true));
		IsDeath();
	}
	// チェンジ用のカウント
	cnt++;
}

void BananaBossDrillState::Exit(void)
{
}

void BananaBossDrillState::AlwaysUpdate(void)
{
}

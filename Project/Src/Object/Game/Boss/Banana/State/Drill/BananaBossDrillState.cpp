#include "BananaBossDrillState.h"

#include "../../../../../../Manager/Net/NetWorkManager.h"
#include "../../../../../../Manager/Sound/SoundManager.h"
#include "../../../../../../Manager/Effect/EffectManager.h"

BananaBossDrillState::BananaBossDrillState(
	const std::function<void(void)>& ownChangeState,
	const std::function<bool(void)>& isOwnState,
	BananaBossBanadrillManager* collOperator,
	const std::function<Vector3(void)> HandPos,
	const std::function<void(void)> PlayFlyAnim,
	const std::function<float(void)> GetAnimRate,
	const std::function<bool(void)> IsAnimeEnd,
	const std::function<void(void)> SetInvisible,
	const std::function<void(void)> SetVisible,
	const std::function<void(void)> Up,
	const std::function<void(void)> SetDown,
	const std::function<void(void)> DefaultChangeState,
	const std::function<void(void)> SetCoolTime
)
	:CharacterStateBase(ownChangeState, isOwnState),
	collOperator(collOperator),
	HandPos(HandPos),
	PlayFlyAnim(PlayFlyAnim),
	IsAnimeEnd(IsAnimeEnd),
	Up(Up), SetDown(SetDown),
	DefaultChangeState(DefaultChangeState),
	SetCoolTime(SetCoolTime)
{
}

void BananaBossDrillState::Enter(void)
{
	first = true;
	num = 0;
	SetCoolTime();
	PlayFlyAnim();
	Net::GetIns().Send(MsgDataBossAttackDrawFlg(MsgDataBossAttackDrawFlg::INFORM_TYPE::ChangeAttackB));
}

void BananaBossDrillState::Update(void)
{
	// アニメーションの再生中なら上昇させる
	if (!IsAnimeEnd) {
		if (GetAnimRate() >= ANIM_RATION) {
			Up();
		}
	}
	// 上昇後
	else {
		// 最初なら透明にし一つ目を落とす
		if (first) {
			SetInvisible();
			collOperator->GetBanadrill(num)->SetOn();
			first = false;
		}
		else {
			if (collOperator->GetBanadrill(num)->GetIsGround()) {
				if (num < NUM) {
					num++;
					collOperator->GetBanadrill(num)->SetOn();
				}
				// 規定数落としたら下に移動し飛ばす
				else if (num == NUM) {
					SetDown();
					PlayFlyAnim();
					num++;
				}
				// 攻撃後ならデフォルトへ
				else {
					DefaultChangeState();
				}
			}
		}
	}
}

void BananaBossDrillState::Exit(void)
{
}

void BananaBossDrillState::AlwaysUpdate(void)
{
}

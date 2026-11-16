#include "BananaBossBoomerangState.h"

#include "../../../../../../Manager/Net/NetWorkManager.h"
#include "../../../../../../Manager/Sound/SoundManager.h"

#include "../../../../../../Utility/Utility.h"

BananaBossBoomerangState::BananaBossBoomerangState(
	BananaBossBanamerang* collOperator,
	const std::function<Vector3(void)> HandPos,
	const std::function<void(void)> PlayStartAnim,
	const std::function<void(void)> PlayLoopAnim,
	const std::function<void(void)> PlayAttackAnim,
	const std::function<void(void)> PlayIdleAnim,
	const std::function<bool(void)> IsAnimeEnd,
	const std::function<void(void)> DefaultChangeState,
	const std::function<void(void)> SetCoolTime
)
	:CharacterStateBase(),
	collOperator(collOperator),
	HandPos(HandPos),
	PlayStartAnim(PlayStartAnim),
	PlayLoopAnim(PlayLoopAnim),
	PlayAttackAnim(PlayAttackAnim),
	PlayIdleAnim(PlayIdleAnim),
	IsAnimeEnd(IsAnimeEnd),
	DefaultChangeState(DefaultChangeState),
	SetCoolTime(SetCoolTime)
{
}

void BananaBossBoomerangState::Enter(void)
{
	isCharging = true;
	isPlayingChargeStart = true;
	elapsedFrames = 0.0f;
	SetCoolTime();
	PlayStartAnim();
	collOperator->SetDrawArea(true);
	Net::GetIns().Send(MsgDataBossAttackDrawFlg(MsgDataBossAttackDrawFlg::INFORM_TYPE::ChangeAttackC));
}

void BananaBossBoomerangState::Update(void)
{
	// 共通のカウント
	elapsedFrames++;
	// 溜めなら
	if (isCharging) {
		// 溜め開始アニメーションが終わっていたらループに変える
		if (isPlayingChargeStart && IsAnimeEnd()) {
			PlayLoopAnim();
			isPlayingChargeStart = false;
		}
		// 攻撃描画を更新
		collOperator->SetScale(elapsedFrames / START_TIME);
		// バナメランを追従
		collOperator->SetBanameranPos(HandPos());
		Net::GetIns().Send(MsgDataBossAttackDraw(MsgDataBossAttackDraw::INFORM_TYPE::ChangeAttackC, HandPos(), elapsedFrames / START_TIME));
		// ため時間を超えてたら
		if (elapsedFrames >= START_TIME) {
			// 攻撃の発生とカウントの初期化
			PlayAttackAnim();
			collOperator->CollSet(true);
			isCharging = false;
			elapsedFrames = 0;
			Snd::GetIns().Play("Boomerang");
		}
	}
	// 攻撃中なら
	else {
		// 回転させる
		collOperator->SetBanameranRot(Quaternion::FromRotationY(Deg2Rad(elapsedFrames * 20.0f)));
		collOperator->SetBanameranPos(Vector3(POS.x * cosf(Deg2Rad(elapsedFrames)), POS.y, POS.z + DIFF * -sinf(Deg2Rad(elapsedFrames))));
		Net::GetIns().Send(MsgDataBossAttackDraw(MsgDataBossAttackDraw::INFORM_TYPE::ChangeAttackC, Vector3(POS.x * cosf(Deg2Rad(elapsedFrames)), POS.y, POS.z + DIFF * -sinf(Deg2Rad(elapsedFrames))), 1.0f, Deg2Rad(elapsedFrames * 20.0f)));
		// 時間が越えてたら終わる
		if (elapsedFrames >= END_TIME) {
			DefaultChangeState();
			return;
		}
		// 攻撃のアニメーションが終了してたらアイドルへ
		if (IsAnimeEnd()) {
			PlayIdleAnim();
		}
	}
}

void BananaBossBoomerangState::Exit(void)
{
	collOperator->Off();
	Net::GetIns().Send(MsgDataBossAttackDrawFlg(MsgDataBossAttackDrawFlg::INFORM_TYPE::ChangeAttackC, false));
	collOperator->SetScale(0.0f);
	Net::GetIns().Send(MsgDataBossAttackDraw(MsgDataBossAttackDraw::INFORM_TYPE::ChangeAttackC, Vector3()));
	Snd::GetIns().Stop("Boomerang");
}

void BananaBossBoomerangState::AlwaysUpdate(void)
{
}

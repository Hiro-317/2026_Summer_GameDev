#include "BananaBossScratchState.h"

#include "../../../../../../Manager/Net/NetWorkManager.h"
#include "../../../../../../Manager/Sound/SoundManager.h"

BananaBossScratchState::BananaBossScratchState(
	BananaScratchCollOperator* collOperator,
	const std::function<void(void)> UpdateFrame,
	const std::function<void(void)> PlayAttackAnim,
	const std::function<float(void)> GetAnimPlayRatio,
	const std::function<bool(void)> IsAnimeEnd,
	const std::function<void(void)> DefaultChangeState,
	const std::function<void(void)> SetCoolTime
)
	:CharacterStateBase(),
	collOperator(collOperator),
	UpdateFrame(UpdateFrame),
	PlayAttackAnim(PlayAttackAnim),
	GetAnimPlayRatio(GetAnimPlayRatio),
	IsAnimeEnd(IsAnimeEnd),
	DefaultChangeState(DefaultChangeState),
	SetCoolTime(SetCoolTime)
{
}

void BananaBossScratchState::Enter(void)
{
	isAttackPending = true;
	SetCoolTime();
	PlayAttackAnim();
	collOperator->SetDrawArea(true);
	Net::GetIns().Send(MsgDataBossAttackDrawFlg(MsgDataBossAttackDrawFlg::INFORM_TYPE::ChangeAttackA));
}

void BananaBossScratchState::Update(void)
{
	// アニメーションが終わっていたらステートを抜ける
	if (GetAnimPlayRatio() >= ATTACK_RATE) {
		// アニメーションの再生割合で生成
		if (isAttackPending) {
			// フレーム位置に攻撃と音を発生
			collOperator->CollSet(true);
			Snd::GetIns().Play("Scratch");
		}
		// 二度目は通さないようにフラグを折る
		isAttackPending = false;

		// 攻撃持続時間の判定
		if (GetAnimPlayRatio() >= END_RATE) {
			collOperator->Off();
			Net::GetIns().Send(MsgDataBossAttackDrawFlg(MsgDataBossAttackDrawFlg::INFORM_TYPE::ChangeAttackA, false));
		}
		// アニメーションが終わったら終わり
		if (IsAnimeEnd()) {

			DefaultChangeState();
			return;
		}
		UpdateFrame();
		collOperator->SetColliderFrame();
	}
	else {
		// 攻撃描画を更新
		collOperator->SetScale(GetAnimPlayRatio() / ATTACK_RATE);
		Net::GetIns().Send(MsgDataBossAttackDraw(MsgDataBossAttackDraw::INFORM_TYPE::ChangeAttackA, Vector3(), GetAnimPlayRatio() / ATTACK_RATE));
	}
}

void BananaBossScratchState::Exit(void)
{
	// 死亡などで攻撃途中に抜けた場合も、攻撃判定と予告表示を残さない。
	collOperator->Off();
	Net::GetIns().Send(MsgDataBossAttackDrawFlg(MsgDataBossAttackDrawFlg::INFORM_TYPE::ChangeAttackA, false));
}

void BananaBossScratchState::AlwaysUpdate(void)
{
}

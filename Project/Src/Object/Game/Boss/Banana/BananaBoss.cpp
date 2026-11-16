#include "BananaBoss.h"

#include "../../../../Application/Application.h"

#include "../../../../Manager/Net/NetWorkManager.h"
#include "../../../../Manager/Font/FontManager.h"
#include "../../../../Manager/Sound/SoundManager.h"

#include "../../../Common/Collider/LineCollider.h"
#include "../../../Common/Collider/CapsuleCollider.h"
#include "../../../Common/Collider/XZCircleCollider.h"

#include "State/Scratch/BananaScratchCollOperator.h"
#include "State/Fire/BananaFireCollOperator.h"
#include "State/Boomerang/BananaBossBanamerang.h"
#include "State/Drill/Banadrill/BananaBossDrillManager.h"

#include "State/Idle/BananaBossIdleState.h"
#include "State/Scratch/BananaBossScratchState.h"
#include "State/Fire/BananaBossFireState.h"
#include "State/Boomerang/BananaBossBoomerangState.h"
#include "State/Drill/BananaBossDrillState.h"
#include "State/Death/BananaBossDeathState.h"

#include "../../../../Scene/Game/GameScene.h"

#include "../../UI/HpBarUI.h"
#include "../../UI/HitUI.h"


BananaBoss::BananaBoss() :
	BossBase(
		"Data/Parameter/Character/Boss/Banagon/",
		"Banana/Banagon")
{
	Snd::GetIns().AddScene("BananaBoss");
	attackCooldownFrames = 120;
}

void BananaBoss::BossLoad(void)
{
	// 共通STATEと固有攻撃の対応: ATTACK_A=ひっかき、B=ブレス、C=バナメラン、D=バナドリル。
	// 登録するコールバックはボスのメンバーを参照するため、ボスと同じ寿命で利用する。
	// アニメーション～～～～～～～～～～～～～～～～～～～～～～～～～～～～

	// アニメーションコントローラーを生成する
	CreateAnimationController();

	// 各種アニメーションを登録する
	AddInFbxAnimation((int)ANIME_TYPE::MAX, ANIME_SPEED_TABLE);

	// ～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～

#pragma region プレイヤーが抱える下位クラスを生成する

	scratchStartPosition = Vector3(MV1GetFramePosition(trans.model, MV1SearchFrame(trans.model, "LowerArm.R")));
	scratchEndPosition = Vector3(MV1GetFramePosition(trans.model, MV1SearchFrame(trans.model, "Ring.01.R")));

	scratchRef = new BananaScratchCollOperator(characterStats, INIT_POS, scratchStartPosition, scratchEndPosition);
	AddChildActor(scratchRef);
	fireRef = new BananaFireCollOperator(characterStats);
	AddChildActor(fireRef);
	banamerangRef = new BananaBossBanamerang(characterStats);
	AddChildActor(banamerangRef);
	banadrillRef = new BananaBossDrillManager(playerPos, targetPlayerIndex, characterStats);
	AddChildActor(banadrillRef);

	AddState(
		STATE::IDLE,
		new BananaBossIdleState(
			// 自分の座標、プレイヤーの座標の読み取り
			trans.pos, trans.rotation, playerPos,
			// クールタイム
			[&]() { return attackCooldownFrames; },
			// ターゲット番号の取得
			[&]() { return targetPlayerIndex; },
			// アイドルアニメーションの再生
			[&]() { AnimePlay((int)ANIME_TYPE::IDLE, true); },
			// ひっかきへの状態遷移関数のポインタ
			[&]() { ChangeState((int)STATE::ATTACK_A); },
			// 吐く攻撃への状態遷移関数のポインタ
			[&]() { ChangeState((int)STATE::ATTACK_B); },
			// バナメランへの状態遷移関数のポインタ
			[&]() { ChangeState((int)STATE::ATTACK_C); },
			// バナドリルへの状態遷移関数のポインタ
			[&]() { ChangeState((int)STATE::ATTACK_D); }
		)
	);

	AddState(
		STATE::ATTACK_A,
		new BananaBossScratchState(
			// コライダへのポインタ
			scratchRef,
			// フレーム座標の更新
			[&]() {
				scratchStartPosition = Vector3(MV1GetFramePosition(trans.model, MV1SearchFrame(trans.model, "LowerArm.R")));
				scratchEndPosition = Vector3(MV1GetFramePosition(trans.model, MV1SearchFrame(trans.model, "Ring.01.R")));
			},
			// 攻撃アニメーションの再生
			[&]() { AnimePlay((int)ANIME_TYPE::SCRATCH, false); },
			// アニメーションの再生割合を取得する関数のポインタ
			[&]() { return GetAnimeRatio(); },
			// アニメーションの終了フラグを取得する関数のポインタ
			[&]() { return IsAnimeEnd(); },
			// 攻撃終了後の状態遷移関数のポインタ
			[&]() { ChangeState((int)STATE::IDLE); },
			// クールタイムの設定
			[&]() { attackCooldownFrames = SCRATCH_COOLTIME; }
		)
	);
	AddState(
		STATE::ATTACK_B,
		new BananaBossFireState(
			// コライダへのポインタ
			fireRef,
			// 攻撃アニメーションの再生
			[&]() { AnimePlay((int)ANIME_TYPE::BREATH, false); },
			// アニメーションの再生割合を取得する関数のポインタ
			[&]() { return GetAnimeRatio(); },
			// アニメーションの終了フラグを取得する関数のポインタ
			[&]() { return IsAnimeEnd(); },
			// 攻撃終了後の状態遷移関数のポインタ
			[&]() { ChangeState((int)STATE::IDLE); },
			// クールタイムの設定
			[&]() { attackCooldownFrames = BREATH_COOLTIME; }
		)
	);
	AddState(
		STATE::ATTACK_C,
		new BananaBossBoomerangState(
			// コライダへのポインタ
			banamerangRef,
			// 手の座標
			[&]() { return Vector3(MV1GetFramePosition(trans.model, MV1SearchFrame(trans.model, "Ring.01.R"))); },
			// 溜めアニメーションの再生
			[&]() { AnimePlay((int)ANIME_TYPE::SCRATCH_START, false); },
			[&]() { AnimePlay((int)ANIME_TYPE::SCRATCH_LOOP, true); },
			// 攻撃アニメーションの再生
			[&]() { AnimePlay((int)ANIME_TYPE::SCRATCH_END, false); },
			// 攻撃後のアニメーションの再生
			[&]() { AnimePlay((int)ANIME_TYPE::IDLE, true); },
			// アニメーションの終了フラグを取得する関数のポインタ
			[&]() { return IsAnimeEnd(); },
			// 攻撃終了後の状態遷移関数のポインタ
			[&]() { ChangeState((int)STATE::IDLE); },
			// クールタイムの設定
			[&]() { attackCooldownFrames = BANAMERANG_COOLTIME; }
		)
	);
	AddState(
		STATE::ATTACK_D,
		new BananaBossDrillState(
			trans.pos, banadrillRef,
			INIT_POS,
			[&]() { AnimePlay((int)ANIME_TYPE::JUMP_LOOP, true); },
			[&]() { return GetIsDraw(); },
			[&]() { SetIsDraw(); },
			[&]() { ChangeState((int)STATE::IDLE); },
			[&]() { attackCooldownFrames = BANADRILL_COOLTIME; }
		)
	);

	AddState(
		STATE::DEATH,
		new BananaBossDeathState(
			// 志望アニメーションの再生
			[&]() { AnimePlay((int)ANIME_TYPE::DEATH_FALL, false); },
			// 死んだフラグ3を立てる
			[&]() {	isDeath = true; }
		)
	);

#pragma endregion


#pragma region 状態設定

#pragma endregion


	// HPバーとヒットUIはBossBase::Loadで共通生成する。

}

void BananaBoss::BossUpdate(void)
{
	// 被ダメージ演出が終わったら待機アニメーションに戻す。
	if (GetAnimePlayType() == (int)ANIME_TYPE::DAMAGE && IsAnimeEnd()) {
		AnimePlay((int)ANIME_TYPE::IDLE, true);
	}
}

void BananaBoss::ReceptionUpdate(void)
{
	// 位置・HPなどは基底側、攻撃範囲などの固有情報はこの後の受信ループで反映する。
	BossBase::ReceptionUpdate();

	// エリアの同期
	while (MsgDataBossAttackDraw* dataPtr = Net::GetIns().GetMsgData<MsgDataBossAttackDraw>(controlSenderId)) {

		// 受け取ったステートの描画
		switch (dataPtr->inform)
		{
		case MsgDataBossAttackDraw::INFORM_TYPE::ChangeAttackA:
		{
			scratchRef->SetScale(dataPtr->scale);
			break;
		}
		case MsgDataBossAttackDraw::INFORM_TYPE::ChangeAttackB:
		{
			fireRef->SetScale(dataPtr->scale);
			break;
		}
		case MsgDataBossAttackDraw::INFORM_TYPE::ChangeAttackC:
		{
			banamerangRef->SetScale(dataPtr->scale);
			banamerangRef->SetBanameranPos(dataPtr->pos);
			banamerangRef->SetBanameranRot(Quaternion::FromRotationY(dataPtr->angle));
			break;
		}
		default:
			break;
		}

		delete dataPtr;
	}
	while (MsgDataBossAttackDrawFlg* dataPtr = Net::GetIns().GetMsgData<MsgDataBossAttackDrawFlg>(controlSenderId)) {

		// 受け取ったステートの描画
		switch (dataPtr->inform)
		{
		case MsgDataBossAttackDrawFlg::INFORM_TYPE::ChangeAttackA:
		{
			if (dataPtr->flg) {
				scratchRef->SetDrawArea(dataPtr->flg);
			}
			else {
				scratchRef->Off();
			}
			break;
		}
		case MsgDataBossAttackDrawFlg::INFORM_TYPE::ChangeAttackB:
		{
			if (dataPtr->flg) {
				fireRef->SetDrawArea(dataPtr->flg);
			}
			else {
				fireRef->Off();
			}
			break;
		}
		case MsgDataBossAttackDrawFlg::INFORM_TYPE::ChangeAttackC:
		{
			if (dataPtr->flg) {
				banamerangRef->SetDrawArea(dataPtr->flg);
			}
			else {
				banamerangRef->Off();
			}
			break;
		}
		default:
			break;
		}

		delete dataPtr;
	}
}

void BananaBoss::BossInit(void)
{
	// 座標・初期ステート・子Actorの初期化は共通処理に任せる。
	ActorBase::SetGravityFlg(false);
}

void BananaBoss::PlayDamage(void)
{
	AnimePlay((int)ANIME_TYPE::DAMAGE, false);
}

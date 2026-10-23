#include "BananaBoss.h"

#include "../../../../Application/Application.h"

#include "../../../../Manager/Net/NetWorkManager.h"
#include "../../../../Manager/Font/FontManager.h"

#include "../../../Common/Collider/LineCollider.h"
#include "../../../Common/Collider/CapsuleCollider.h"
#include "../../../Common/Collider/XZCircleCollider.h"

#include "State/Idle/BananaBossIdleState.h"
#include "State/Scratch/BananaBossScratchState.h"
#include "State/Fire/BananaBossFireState.h"
#include "State/Boomerang/BananaBossBoomerangState.h"
#include "State/Death/BananaBossDeathState.h"

#include "../../../../Scene/Game/GameScene.h"

#include "../../UI/HpBarUI.h"
#include "../../UI/HitUI.h"


BananaBoss::BananaBoss() :
	BossBase(
		"Data/Parameter/Character/Boss/Banagon/",
		"Banana/Banagon")
{

	coolTime = 120;
}

void BananaBoss::BossLoad(void)
{
	// アニメーション～～～～～～～～～～～～～～～～～～～～～～～～～～～～

	// アニメーションコントローラーを生成する
	CreateAnimationController();

	// 各種アニメーションを登録する
	AddInFbxAnimation((int)ANIME_TYPE::MAX, ANIME_SPEED_TABLE);

	// ～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～

#pragma region プレイヤーが抱える下位クラスを生成する

	StartFrame = Vector3(MV1GetFramePosition(trans.model, MV1SearchFrame(trans.model, "LowerArm.R")));
	EndFrame = Vector3(MV1GetFramePosition(trans.model, MV1SearchFrame(trans.model, "Ring.01.R")));

	scratchRef = new BananaScratchCollOperator(controlSenderId, characterStats, INIT_POS, StartFrame, EndFrame);
	AddChildActor(scratchRef);
	fireRef = new BananaFireCollOperator(controlSenderId, characterStats);
	AddChildActor(fireRef);
	banamerangRef = new BananaBossBanamerang(controlSenderId, characterStats);
	AddChildActor(banamerangRef);

	AddState(
		STATE::IDLE,
		new BananaBossIdleState(
			// 自分の座標、プレイヤーの座標の読み取り
			trans.pos, trans.rotation, playerPos,
			// クールタイム
			[&]() { return coolTime; },
			// ターゲット番号の取得
			[&]() { return targetNum; },
			// アイドルアニメーションの再生
			[&]() { AnimePlay((int)ANIME_TYPE::IDLE, true); },
			// ひっかきへの状態遷移関数のポインタ
			[&]() { ChangeState((int)STATE::ATTACK_A); },
			// 吐く攻撃への状態遷移関数のポインタ
			[&]() { ChangeState((int)STATE::ATTACK_B); },
			// バナメランへの状態遷移関数のポインタ
			[&]() { ChangeState((int)STATE::ATTACK_C); }//,
			//// ひとつ追従への状態遷移関数のポインタ
			//[&]() { ChangeState((int)STATE::ATTACK_D); },
			//// たくさん追従への状態遷移関数のポインタ
			//[&]() { ChangeState((int)STATE::ATTACK_E); },
			//// たくさんランダムへの状態遷移関数のポインタ
			//[&]() { ChangeState((int)STATE::ATTACK_F); }
		)
	);

	AddState(
		STATE::ATTACK_A,
		new BananaBossScratchState(
			// コライダへのポインタ
			scratchRef,
			// フレーム座標の更新
			[&]() {
				StartFrame = Vector3(MV1GetFramePosition(trans.model, MV1SearchFrame(trans.model, "LowerArm.R")));
				EndFrame = Vector3(MV1GetFramePosition(trans.model, MV1SearchFrame(trans.model, "Ring.01.R")));
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
			[&]() { coolTime = SCRATCH_COOLTIME; }
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
			[&]() { coolTime = SCRATCH_COOLTIME; }
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
			[&]() { coolTime = SCRATCH_COOLTIME; }
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


#pragma region UI生成
	// HPバー生成
	AddChildActor(
		new HpBarUI(
			"Data/Image/Game/UI/Boss/HP/" + HP_FRAME_IMAGE_NAME,
			"Data/Image/Game/UI/Boss/HP/" + HP_IMAGE_NAME,
			"Data/Image/Game/UI/Boss/HP/" + HP_LOST_IMAGE_NAME,

			Vector2I(667, 50),
			Vector2I(629, 50),

			HP_UI_POS,

			characterStats
		)
	);

	AddChildActor(new HitUI());
#pragma endregion

}

void BananaBoss::ReceptionUpdate(void)
{
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
	// 位置を初期位置にする
	trans.pos = INIT_POS;
	trans.rotation = Quaternion::FromEulerXZY(INIT_ROT);

	// 重力無効
	ActorBase::SetGravityFlg(false);

	// 初期状態を移動状態にする
	ChangeState(static_cast<int>(STATE::IDLE));

	for (ActorBase* c : GetChildActors()) { c->Init(); }
}

void BananaBoss::PlayDamage(void)
{
	AnimePlay((int)ANIME_TYPE::DAMAGE, false);
}

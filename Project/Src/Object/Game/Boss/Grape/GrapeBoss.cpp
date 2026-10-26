#include "GrapeBoss.h"

#include "../../../../Application/Application.h"

#include "../../../../Manager/Net/NetWorkManager.h"
#include "../../../../Manager/Sound/SoundManager.h"

#include "Weapon/GrapeBossWeaponManager.h"
#include "Weapon/Straight/GrapeBossStraight.h"

#include "../../../Common/Collider/LineCollider.h"
#include "../../../Common/Collider/CapsuleCollider.h"
#include "../../../Common/Collider/XZCircleCollider.h"

#include "State/Idle/GrapeBossIdleState.h"
#include "State/Move/GrapeBossMoveState.h"
#include "State/KickDown/GrapeBossKickDownState.h"
#include "State/KickDown/GrapeKickDownCollOperator.h"
#include "State/Straight/GrapeBossStraightState.h"
#include "State/Stamp/GrapeBossStampState.h"
#include "State/Stamp/GrapeStampCollOperator.h"
#include "State/Single/GrapeBossSingleState.h"
#include "State/Stalker/GrapeBossStalkerState.h"
#include "State/Random/GrapeBossRandomState.h"
#include "State/Death/GrapeBossDeathState.h"

#include "../../../../Scene/Game/GameScene.h"

#include "../../UI/HpBarUI.h"
#include "../../UI/HitUI.h"


GrapeBoss::GrapeBoss() :
	BossBase(
		"Data/Parameter/Character/Boss/Grape/",
		"Grape/GrapeModel"
	)
{
	coolTime = 120;
	Snd::GetIns().AddScene("GrapeBoss");
}

void GrapeBoss::BossLoad(void)
{
	// アニメーション～～～～～～～～～～～～～～～～～～～～～～～～～～～～

	// アニメーションコントローラーを生成する
	CreateAnimationController();

	// 各種アニメーションを登録する
	AddInFbxAnimation((int)ANIME_TYPE::MAX, ANIME_SPEED_TABLE);

	// ～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～

#pragma region プレイヤーが抱える下位クラスを生成する

	AddChildActor(new GrapeBossWeaponManager(controlSenderId, characterStats));
	AddChildActor(new GrapeKickDownCollOperator(controlSenderId, characterStats));
	AddChildActor(new GrapeStampCollOperator(controlSenderId, characterStats));

#pragma endregion


#pragma region 状態設定

	AddState(
		STATE::IDLE,
		new GrapeBossIdleState(
			// 自分の座標
			trans,
			// プレイヤーの座標の読み取り
			playerPos,

			// クールタイム
			[&]() { return coolTime; },
			// ターゲット番号の取得
			[&]() { return targetNum; },

			// アイドルアニメーションの再生
			[&]() { AnimePlay(ANIME_TYPE::IDLE); },
			// 歩きアニメーションの再生
			[&]() { AnimePlay(ANIME_TYPE::WALK); },

			// 移動への状態遷移関数のポインタ
			[&]() { ChangeState(STATE::MOVE); },
			// 踏みつけへの状態遷移関数のポインタ
			[&]() { ChangeState(STATE::ATTACK_A); },
			// 投擲への状態遷移関数のポインタ
			[&]() { ChangeState(STATE::ATTACK_B); },
			// スタンプへの状態遷移関数のポインタ
			[&]() { ChangeState(STATE::ATTACK_C); },
			// ひとつ追従への状態遷移関数のポインタ
			[&]() { ChangeState(STATE::ATTACK_D); },
			// たくさん追従への状態遷移関数のポインタ
			[&]() { ChangeState(STATE::ATTACK_E); },
			// たくさんランダムへの状態遷移関数のポインタ
			[&]() { ChangeState(STATE::ATTACK_F); }
		)
	);

	AddState(
		STATE::MOVE,
		new GrapeBossMoveState(
			// 移動量
			MOVE_SPEED,
			// 自分の座標と角度、プレイヤーの座標の読み取り
			trans,
			playerPos,
			// ターゲット番号の取得
			[&]() { return targetNum; },
			// 走りアニメーションの再生
			[&]() { AnimePlay(ANIME_TYPE::RUN); },
			// 移動後攻撃に状態遷移関数のポインタ
			[&]() { ChangeState(STATE::ATTACK_A); }
		)
	);

	AddState(
		STATE::ATTACK_A,
		new GrapeBossKickDownState(
			// 自分の座標と角度とローカル角度
			trans, MODEL_LOCAL_ROT,
			// 攻撃の種類の情報
			ChildActorSerch<GrapeBossWeaponManager>()->GetWeapons(WeaponType::KickBomb),
			// モデルの拡大率
			MODEL_SCALE,
			// プレイヤーの座標の読み取り
			playerPos,
			// 当たり判定オペレータの参照
			ChildActorSerch<GrapeKickDownCollOperator>(),
			// アニメーションの再生関数のポインタ
			[&]() { return targetNum; },
			// アニメーションの再生関数のポインタ
			[&]() { AnimePlay((int)ANIME_TYPE::KICKDOWN); },
			// アニメーションの再生割合を取得する関数のポインタ 
			[&]() { return GetAnimeRatio(); },
			// アニメーションの終了フラグを取得する関数のポインタ
			[&]() { return IsAnimeEnd(); },
			// 攻撃終了後の状態遷移関数のポインタ
			[&]() { ChangeState((int)STATE::IDLE); },
			// クールタイムの設定
			[&]() { coolTime = KICKDOWN_COOLTIME; }
		)
	);
	AddState(
		STATE::ATTACK_B,
		new GrapeBossStraightState(
			// 自分の座標と角度
			trans,
			// 攻撃の種類の情報
			ChildActorSerch<GrapeBossWeaponManager>()->GetWeapons(WeaponType::Straight),
			// プレイヤーの座標
			playerPos,
			// プレイヤーのターゲット番号
			[&]() { return targetNum; },
			// アニメーションの再生関数のポインタ
			[&]() { AnimePlay((int)ANIME_TYPE::FORWARD_THROW, false); },
			// アニメーションの再生割合を取得する関数のポインタ 
			[&]() { return GetAnimeRatio(); },
			// アニメーションの終了フラグを取得する関数のポインタ
			[&]() { return IsAnimeEnd(); },
			// 攻撃終了後の状態遷移関数のポインタ
			[&]() { ChangeState((int)STATE::IDLE); },
			// クールタイムの設定
			[&]() { coolTime = STRAIGHT_COOLTIME; }
		)
	);
	AddState(
		STATE::ATTACK_C,
		new GrapeBossStampState(
			// 自分の座標と角度
			trans, 
			isGround,
			// 攻撃の種類の情報
			ChildActorSerch<GrapeBossWeaponManager>()->GetWeapons(WeaponType::StampBomb),
			// プレイヤーの座標
			playerPos,
			ChildActorSerch<GrapeStampCollOperator>(),
			// ジャンプ始めアニメーションのタイプ
			(int)ANIME_TYPE::JUMP_START,
			(int)ANIME_TYPE::STAMP,
			// プレイヤーのターゲット番号
			[&]() { return targetNum; },
			// アニメーションの再生関数のポインタ
			[&]() { AnimePlay((int)ANIME_TYPE::JUMP_START, false); },
			[&]() { AnimePlay((int)ANIME_TYPE::JUMP_LOOP, true); },
			[&]() { AnimePlay((int)ANIME_TYPE::STAMP, false); },
			// アニメーションのスタンプタイプ
			[&]() { return GetAnimePlayType(); },
			// アニメーションの再生割合を取得する関数のポインタ 
			[&]() { return GetAnimeRatio(); },
			// アニメーションの終了フラグを取得する関数のポインタ
			[&]() { return IsAnimeEnd(); },
			// 攻撃終了後の状態遷移関数のポインタ
			[&]() { ChangeState((int)STATE::IDLE); },
			// ボス自体のコライダの消滅
			[&]() { SetJudgeFlg(false); },
			// ボス自体のコライダの再生
			[&]() { SetJudgeFlg(true); },
			// クールタイムの設定
			[&]() { coolTime = STAMP_COOLTIME; }
		)
	);
	AddState(
		STATE::ATTACK_D,
		new GrapeBossSingleState(
			// プレイヤーの座標と角度
			playerPos,
			// 攻撃の種類の情報
			ChildActorSerch<GrapeBossWeaponManager>()->GetWeapons(WeaponType::SingleBomb),
			// プレイヤーのターゲット番号
			[&]() { return targetNum; },
			// アニメーションの再生関数のポインタ
			[&]() { AnimePlay((int)ANIME_TYPE::TOSS, false); },
			// アニメーションの再生割合を取得する関数のポインタ 
			[&]() { return GetAnimeRatio(); },
			// アニメーションの終了フラグを取得する関数のポインタ
			[&]() { return IsAnimeEnd(); },
			// 攻撃終了後の状態遷移関数のポインタ
			[&]() { ChangeState((int)STATE::IDLE); },
			// クールタイムの設定
			[&]() { coolTime = SINGLE_COOLTIME; }
		)
	);
	AddState(
		STATE::ATTACK_E,
		new GrapeBossStalkerState(
			// プレイヤーの座標と角度
			playerPos,
			// 攻撃の種類の情報
			ChildActorSerch<GrapeBossWeaponManager>()->GetWeapons(WeaponType::StalkerBomb),
			// プレイヤーのターゲット番号
			[&]() { return targetNum; },
			// アニメーションの再生関数のポインタ
			[&]() { AnimePlay((int)ANIME_TYPE::TOSS, false); },
			// アニメーションの再生割合を取得する関数のポインタ 
			[&]() { return GetAnimeRatio(); },
			// アニメーションの終了フラグを取得する関数のポインタ
			[&]() { return IsAnimeEnd(); },
			// 攻撃終了後の状態遷移関数のポインタ
			[&]() { ChangeState((int)STATE::IDLE); },
			// クールタイムの設定
			[&]() { coolTime = STAMP_COOLTIME; }
		)
	);
	AddState(
		STATE::ATTACK_F,
		new GrapeBossRandomState(
			// 座標の参照
			trans.pos,
			// 攻撃の種類の情報
			ChildActorSerch<GrapeBossWeaponManager>()->GetWeapons(WeaponType::RandomBomb),
			// アニメーションの再生関数のポインタ
			[&]() { AnimePlay((int)ANIME_TYPE::TOSS, false); },
			// アニメーションの再生割合を取得する関数のポインタ 
			[&]() { return GetAnimeRatio(); },
			// アニメーションの終了フラグを取得する関数のポインタ
			[&]() { return IsAnimeEnd(); },
			// 攻撃終了後の状態遷移関数のポインタ
			[&]() { ChangeState((int)STATE::IDLE); },
			// クールタイムの設定
			[&]() { coolTime = RANDOM_COOLTIME; }
		)
	);

	AddState(
		STATE::DEATH,
		new GrapeBossDeathState(
			// ボス死亡アニメーション
			[&]() { AnimePlay((int)ANIME_TYPE::DEATH, false); },
			// アニメーションの全体時間
			[&]() { return GetAnimeTotalTime(); },
			// アニメーションの終了フラグを取得する関数のポインタ
			[&]() { return IsAnimeEnd(); },
			// 死んだフラグを立てる
			[&]() {	isDeath = true; }
		)
	);

#pragma endregion

#pragma region UIの登録と設定

	// HPバーUIの登録
	AddChildActor(
		new HpBarUI(
			"Data/Image/Game/UI/Boss/HP/" + HP_FRAME_IMAGE_NAME,
			"Data/Image/Game/UI/Boss/HP/" + HP_IMAGE_NAME,
			"Data/Image/Game/UI/Boss/HP/" + HP_LOST_IMAGE_NAME,
			
			Vector2I(667,50),
			Vector2I(629,50),

			HP_UI_POS,

			characterStats
		)
	);

	// ヒットUIの生成 / 登録
	hitUiRef = new HitUI();
	AddChildActor(hitUiRef);

#pragma endregion 

}

void GrapeBoss::BossUpdate(void)
{
	if (coolTime > 0.0f) {
		coolTime -= TimeScale::Get();
		if (coolTime < 0.0f) { coolTime = 0.0f; }
	}
}

void GrapeBoss::ReceptionUpdate(void)
{
	BossBase::ReceptionUpdate();

	// エリアの同期
	while (MsgDataBossAttackDraw* dataPtr = Net::GetIns().GetMsgData<MsgDataBossAttackDraw>(controlSenderId)) {

		// 受け取ったステートの描画
		switch (dataPtr->inform)
		{
		case MsgDataBossAttackDraw::INFORM_TYPE::ChangeAttackA:
		{
			ChildActorSerch<GrapeKickDownCollOperator>()->SetPos(dataPtr->pos);
			ChildActorSerch<GrapeKickDownCollOperator>()->SetScale(dataPtr->scale);
			break;
		}
		case MsgDataBossAttackDraw::INFORM_TYPE::ChangeAttackC:
		{
			ChildActorSerch<GrapeStampCollOperator>()->SetPos(dataPtr->pos);
			ChildActorSerch<GrapeStampCollOperator>()->SetScale(dataPtr->scale);
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
			ChildActorSerch<GrapeKickDownCollOperator>()->SetDrawArea(dataPtr->flg);
			break;
		}
		case MsgDataBossAttackDrawFlg::INFORM_TYPE::ChangeAttackC:
		{
			if (dataPtr->flg) {
				ChildActorSerch<GrapeStampCollOperator>()->DrawAreaOn();
			}
			else {
				ChildActorSerch<GrapeStampCollOperator>()->Off();
			}
			break;
		}
		default:
			break;
		}

		delete dataPtr;
	}
	while (MsgDataBossBombInform* dataPtr = Net::GetIns().GetMsgData<MsgDataBossBombInform>(controlSenderId)) {

		// 受け取ったボムの起動
		switch (dataPtr->inform)
		{
		case MsgDataBossBombInform::INFORM_TYPE::Straight:
		{
			auto& weapon = ChildActorSerch<GrapeBossWeaponManager>()->GetWeapons(WeaponType::Straight)[dataPtr->index];
			weapon.weaponIns->SetStartPos(dataPtr->pos);
			dynamic_cast<GrapeBossStraight*>(weapon.weaponIns)->SetStartDir(dataPtr->moveDir);
			weapon.live = true;
			break;
		}
		case MsgDataBossBombInform::INFORM_TYPE::KickBomb:
		{
			auto& weapon = ChildActorSerch<GrapeBossWeaponManager>()->GetWeapons(WeaponType::KickBomb)[dataPtr->index];
			weapon.weaponIns->SetStartPos(dataPtr->pos);
			weapon.weaponIns->SetViewPosCircle();
			weapon.live = true;
			break;
		}
		case MsgDataBossBombInform::INFORM_TYPE::StampBomb:
		{
			auto& weapon = ChildActorSerch<GrapeBossWeaponManager>()->GetWeapons(WeaponType::StampBomb)[dataPtr->index];
			weapon.weaponIns->SetStartPos(dataPtr->pos);
			weapon.weaponIns->SetViewPosCircle();
			weapon.live = true;
			break;
		}
		case MsgDataBossBombInform::INFORM_TYPE::SingleBomb:
		{
			auto& weapon = ChildActorSerch<GrapeBossWeaponManager>()->GetWeapons(WeaponType::SingleBomb)[dataPtr->index];
			weapon.weaponIns->SetStartPos(dataPtr->pos);
			weapon.weaponIns->SetViewPosCircle();
			weapon.live = true; 
			break;
		}
		case MsgDataBossBombInform::INFORM_TYPE::StalkerBomb:
		{
			auto& weapon = ChildActorSerch<GrapeBossWeaponManager>()->GetWeapons(WeaponType::StalkerBomb)[dataPtr->index];
			weapon.weaponIns->SetStartPos(dataPtr->pos);
			weapon.weaponIns->SetViewPosCircle();
			weapon.live = true; 
			break;
		}
		case MsgDataBossBombInform::INFORM_TYPE::RandomBomb:
		{
			auto& weapon = ChildActorSerch<GrapeBossWeaponManager>()->GetWeapons(WeaponType::RandomBomb)[dataPtr->index];
			weapon.weaponIns->SetStartPos(dataPtr->pos);
			weapon.weaponIns->SetViewPosCircle();
			weapon.live = true;
			break;
		}
		default:
			break;
		}

		delete dataPtr;
	}
}

void GrapeBoss::PlayDamage(void)
{
	AnimePlay((int)ANIME_TYPE::DAMAGE, false);
}

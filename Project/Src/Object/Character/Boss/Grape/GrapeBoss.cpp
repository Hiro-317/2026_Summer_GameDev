#include "GrapeBoss.h"

#include "../../../../Application/Application.h"

#include "../../../../Manager/Net/NetWorkManager.h"
#include "../../../../Manager/Font/FontManager.h"

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

#include "../../../../Manager/Effect/EffectManager.h"

#include "../../../UI/CharacterHpUI/CharacterHpUI.h"
#include "../../../UI/HitUI/HitUI.h"


GrapeBoss::GrapeBoss(const std::vector<const Vector3*> playerPos, const std::vector<const bool*> playerLive) :
	BossBase(
		"HP",
		"AttackPower",
		"DefensePower",
		"MoveSpeed",
		"Data/Parameter/Character/Boss/Grape/",
		"Grape/GrapeModel",

		playerPos, playerLive)
{

	coolTime = 120;
}

void GrapeBoss::PlayerLoad(void)
{
	// アニメーション～～～～～～～～～～～～～～～～～～～～～～～～～～～～

// アニメーションコントローラーを生成する
	CreateAnimationController();

	// 各種アニメーションを登録する
	AddInFbxAnimation((int)ANIME_TYPE::MAX, ANIME_SPEED_TABLE);

	// ～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～

#pragma region 当たり判定情報設定

	// 当たり判定を生成する（カプセルコライダー）
	ColliderCreate(
		new CapsuleCollider(
			COLLIDER_TAG::BOSS,
			CAPSULE_COLLIDER_START_POS, CAPSULE_COLLIDER_END_POS,
			CAPSULE_COLLIDER_RADIUS,
			CAPSULE_COLLIDER_ENOUGH_DISTANCE
		)
	);

#pragma endregion


#pragma region プレイヤーが抱える下位クラスを生成する

	subObjArray.push_back(new GrapeBossWeaponManager(operatorSenderId, characterStats));
	subObjArray.push_back(new GrapeKickDownCollOperator(operatorSenderId, characterStats));
	subObjArray.push_back(new GrapeStampCollOperator(operatorSenderId, characterStats));

#pragma endregion


#pragma region 状態設定

	AddState(
		static_cast<int>(STATE::IDLE),
		new GrapeBossIdleState(
			// 自分の状態に遷移する関数
			[&]() { state = static_cast<int>(STATE::IDLE); },
			// 自分の状態かどうかを返す関数
			[&]() { return state == static_cast<int>(STATE::IDLE); },
			// 自分の座標、プレイヤーの座標の読み取り
			trans.pos, trans.angle, playerPos,
			// クールタイム
			[&]() { return coolTime; },
			[&]() { return targetNum; },
			[&]() { AnimePlay((int)ANIME_TYPE::IDLE, true); },
			[&]() { AnimePlay((int)ANIME_TYPE::WALK, true); },
			// 移動への状態遷移関数のポインタ
			[&]() { ChangeState((int)STATE::MOVE); },
			// 踏みつけへの状態遷移関数のポインタ
			[&]() { ChangeState((int)STATE::ATTACK_A); },
			// 投擲への状態遷移関数のポインタ
			[&]() { ChangeState((int)STATE::ATTACK_B); },
			// スタンプへの状態遷移関数のポインタ
			[&]() { ChangeState((int)STATE::ATTACK_C); },
			// ひとつ追従への状態遷移関数のポインタ
			[&]() { ChangeState((int)STATE::ATTACK_D); },
			// たくさん追従への状態遷移関数のポインタ
			[&]() { ChangeState((int)STATE::ATTACK_E); },
			// たくさんランダムへの状態遷移関数のポインタ
			[&]() { ChangeState((int)STATE::ATTACK_F); }
		)
	);
	AddState(
		static_cast<int>(STATE::MOVE),
		new GrapeBossMoveState(
			// 自分の状態に遷移する関数
			[&]() { state = static_cast<int>(STATE::MOVE); },
			// 自分の状態かどうかを返す関数
			[&]() { return state == static_cast<int>(STATE::MOVE); },
			// 移動量
			MOVE_SPEED,
			// 自分の座標と角度、プレイヤーの座標の読み取り
			trans.pos, trans.angle, playerPos,
			[&]() { return targetNum; },
			[&]() { AnimePlay((int)ANIME_TYPE::RUN, true); },
			// 移動後攻撃に状態遷移関数のポインタ
			[&]() { ChangeState((int)STATE::ATTACK_A); }
		)
	);

	AddState(
		static_cast<int>(STATE::ATTACK_A),
		new GrapeBossKickDownState(
			// 自分の状態に遷移する関数
			[&]() { state = static_cast<int>(STATE::ATTACK_A); },
			// 自分の状態かどうかを返す関数
			[&]() { return state == static_cast<int>(STATE::ATTACK_A); },
			// 自分の座標と角度
			trans.pos, trans.angle, MODEL_LOCAL_ROT,
			// 攻撃の種類の情報
			SubObjSerch<GrapeBossWeaponManager>()->GetWeapons(WeaponType::KickBomb),
			MODEL_SCALE,
			SubObjSerch<GrapeKickDownCollOperator>(),
			// アニメーションの再生関数のポインタ
			[&]() { AnimePlay((int)ANIME_TYPE::KICKDOWN, false); },
			// アニメーションの再生割合を取得する関数のポインタ 
			[&]() { return GetAnimeRatio(); },
			// アニメーションの終了フラグを取得する関数のポインタ
			[&]() { return IsAnimeEnd(); },
			// 攻撃終了後の状態遷移関数のポインタ (今回は移動状態に遷移するようにする）
			[&]() { ChangeState((int)STATE::IDLE); },
			[&]() { coolTime = 120; }
		)
	);
	AddState(
		static_cast<int>(STATE::ATTACK_B),
		new GrapeBossStraightState(
			// 自分の状態に遷移する関数
			[&]() { state = static_cast<int>(STATE::ATTACK_B); },
			// 自分の状態かどうかを返す関数
			[&]() { return state == static_cast<int>(STATE::ATTACK_B); },
			// 自分の座標と角度
			trans.pos, trans.angle,
			// 攻撃の種類の情報
			SubObjSerch<GrapeBossWeaponManager>()->GetWeapons(WeaponType::Straight),
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
			// 攻撃終了後の状態遷移関数のポインタ (今回は移動状態に遷移するようにする）
			[&]() { ChangeState((int)STATE::IDLE); },
			[&]() { coolTime = 100; }
		)
	);
	AddState(
		static_cast<int>(STATE::ATTACK_C),
		new GrapeBossStampState(
			// 自分の状態に遷移する関数
			[&]() { state = static_cast<int>(STATE::ATTACK_C); },
			// 自分の状態かどうかを返す関数
			[&]() { return state == static_cast<int>(STATE::ATTACK_C); },
			// 自分の座標と角度
			trans.pos, trans.angle, isGround,
			// 攻撃の種類の情報
			SubObjSerch<GrapeBossWeaponManager>()->GetWeapons(WeaponType::StampBomb),
			// プレイヤーの座標
			playerPos,
			SubObjSerch<GrapeStampCollOperator>(),
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
			// 攻撃終了後の状態遷移関数のポインタ (今回は移動状態に遷移するようにする）
			[&]() { ChangeState((int)STATE::IDLE); },
			[&]() { SetJudge(false); },
			[&]() { SetJudge(true); },
			[&]() { coolTime = 180; }
		)
	);
	AddState(
		static_cast<int>(STATE::ATTACK_D),
		new GrapeBossSingleState(
			// 自分の状態に遷移する関数
			[&]() { state = static_cast<int>(STATE::ATTACK_D); },
			// 自分の状態かどうかを返す関数
			[&]() { return state == static_cast<int>(STATE::ATTACK_D); },
			// プレイヤーの座標と角度
			playerPos,
			// 攻撃の種類の情報
			SubObjSerch<GrapeBossWeaponManager>()->GetWeapons(WeaponType::SingleBomb),
			// プレイヤーのターゲット番号
			[&]() { return targetNum; },
			// アニメーションの再生関数のポインタ
			[&]() { AnimePlay((int)ANIME_TYPE::TOSS, false); },
			// アニメーションの再生割合を取得する関数のポインタ 
			[&]() { return GetAnimeRatio(); },
			// アニメーションの終了フラグを取得する関数のポインタ
			[&]() { return IsAnimeEnd(); },
			// 攻撃終了後の状態遷移関数のポインタ (今回は移動状態に遷移するようにする）
			[&]() { ChangeState((int)STATE::IDLE); },
			[&]() { coolTime = 180; }
		)
	);
	AddState(
		static_cast<int>(STATE::ATTACK_E),
		new GrapeBossStalkerState(
			// 自分の状態に遷移する関数
			[&]() { state = static_cast<int>(STATE::ATTACK_E); },
			// 自分の状態かどうかを返す関数
			[&]() { return state == static_cast<int>(STATE::ATTACK_E); },
			// プレイヤーの座標と角度
			playerPos,
			// 攻撃の種類の情報
			SubObjSerch<GrapeBossWeaponManager>()->GetWeapons(WeaponType::StalkerBomb),
			// プレイヤーのターゲット番号
			[&]() { return targetNum; },
			// アニメーションの再生関数のポインタ
			[&]() { AnimePlay((int)ANIME_TYPE::TOSS, false); },
			// アニメーションの再生割合を取得する関数のポインタ 
			[&]() { return GetAnimeRatio(); },
			// アニメーションの終了フラグを取得する関数のポインタ
			[&]() { return IsAnimeEnd(); },
			// 攻撃終了後の状態遷移関数のポインタ (今回は移動状態に遷移するようにする）
			[&]() { ChangeState((int)STATE::IDLE); },
			[&]() { coolTime = 180; }
		)
	);
	AddState(
		static_cast<int>(STATE::ATTACK_F),
		new GrapeBossRandomState(
			// 自分の状態に遷移する関数
			[&]() { state = static_cast<int>(STATE::ATTACK_F); },
			// 自分の状態かどうかを返す関数
			[&]() { return state == static_cast<int>(STATE::ATTACK_F); },
			// 座標
			trans.pos,
			// 攻撃の種類の情報
			SubObjSerch<GrapeBossWeaponManager>()->GetWeapons(WeaponType::RandomBomb),
			// プレイヤーのターゲット番号
			[&]() { return targetNum; },
			// アニメーションの再生関数のポインタ
			[&]() { AnimePlay((int)ANIME_TYPE::TOSS, false); },
			// アニメーションの再生割合を取得する関数のポインタ 
			[&]() { return GetAnimeRatio(); },
			// アニメーションの終了フラグを取得する関数のポインタ
			[&]() { return IsAnimeEnd(); },
			// 攻撃終了後の状態遷移関数のポインタ (今回は移動状態に遷移するようにする）
			[&]() { ChangeState((int)STATE::IDLE); },
			[&]() { coolTime = 180; }
		)
	);

	AddState(
		static_cast<int>(STATE::DEATH),
		new GrapeBossDeathState(
			// 自分の状態に遷移する関数
			[&]() { state = static_cast<int>(STATE::DEATH); },
			// 自分の状態かどうかを返す関数
			[&]() { return state == static_cast<int>(STATE::DEATH); },
			// ボスのサイズ
			trans.scale, MODEL_SCALE,
			// 死んだフラグを立てる
			[&]() {	SetIsDeath(true); }
		)
	);

#pragma endregion


#pragma region UI生成
	// HPバー生成
	ui_ArrayIns.emplace_back(
		new CharacterHpUI(
			characterStats.hp,
			characterStats.hpMax.Value(),
			HP_FRAME_IMAGE_NAME,
			HP_IMAGE_NAME,
			HP_LOST_IMAGE_NAME,
			HP_IMAGE_SIZE,
			HP_GAUGE_OFFSET,
			HP_UI_POS,
			FILE_PATH_TYPE::BOSS_HP,
			"GrapeBoss"
		)
	);

	ui_ArrayIns.emplace_back(new HitUI(trans.pos));
#pragma endregion

}

void GrapeBoss::ReceptionUpdate(void)
{
	BossBase::ReceptionUpdate();

	// エリアの同期
	while (MsgDataBossAttackDraw* dataPtr = Net::GetIns().GetMsgData<MsgDataBossAttackDraw>(operatorSenderId)) {

		// 受け取ったステートの描画
		switch (dataPtr->inform)
		{
		case MsgDataBossAttackDraw::INFORM_TYPE::ChangeAttackA:
		{
			SubObjSerch<GrapeKickDownCollOperator>()->SetPos(dataPtr->pos);
			SubObjSerch<GrapeKickDownCollOperator>()->SetScale(dataPtr->scale.x);
			break;
		}
		case MsgDataBossAttackDraw::INFORM_TYPE::ChangeAttackC:
		{
			SubObjSerch<GrapeStampCollOperator>()->SetPos(dataPtr->pos);
			SubObjSerch<GrapeStampCollOperator>()->SetScale(dataPtr->scale.x);
			break;
		}
		default:
			break;
		}

		delete dataPtr;
	}
	while (MsgDataBossAttackDrawFlg* dataPtr = Net::GetIns().GetMsgData<MsgDataBossAttackDrawFlg>(operatorSenderId)) {

		// 受け取ったステートの描画
		switch (dataPtr->inform)
		{
		case MsgDataBossAttackDrawFlg::INFORM_TYPE::ChangeAttackA:
		{
			SubObjSerch<GrapeKickDownCollOperator>()->SetDrawArea(dataPtr->flg);
			break;
		}
		case MsgDataBossAttackDrawFlg::INFORM_TYPE::ChangeAttackC:
		{
			SubObjSerch<GrapeStampCollOperator>()->SetDrawArea(dataPtr->flg);
			break;
		}
		default:
			break;
		}

		delete dataPtr;
	}
	while (MsgDataBossBombInform* dataPtr = Net::GetIns().GetMsgData<MsgDataBossBombInform>(operatorSenderId)) {

		// 受け取ったボムの起動
		switch (dataPtr->inform)
		{
		case MsgDataBossBombInform::INFORM_TYPE::Straight:
		{
			auto& weapon = SubObjSerch<GrapeBossWeaponManager>()->GetWeapons(WeaponType::Straight)[dataPtr->index];
			weapon.weaponIns->SetStartPos(dataPtr->pos);
			dynamic_cast<GrapeBossStraight*>(weapon.weaponIns)->SetStartDir(dataPtr->moveDir);
			weapon.live = true;
			break;
		}
		case MsgDataBossBombInform::INFORM_TYPE::KickBomb:
		{
			auto& weapon = SubObjSerch<GrapeBossWeaponManager>()->GetWeapons(WeaponType::KickBomb)[dataPtr->index];
			weapon.weaponIns->SetStartPos(dataPtr->pos);
			weapon.weaponIns->SetViewPosCircle();
			weapon.live = true;
			break;
		}
		case MsgDataBossBombInform::INFORM_TYPE::StampBomb:
		{
			auto& weapon = SubObjSerch<GrapeBossWeaponManager>()->GetWeapons(WeaponType::StampBomb)[dataPtr->index];
			weapon.weaponIns->SetStartPos(dataPtr->pos);
			weapon.weaponIns->SetViewPosCircle();
			weapon.live = true;
			break;
		}
		case MsgDataBossBombInform::INFORM_TYPE::SingleBomb:
		{
			auto& weapon = SubObjSerch<GrapeBossWeaponManager>()->GetWeapons(WeaponType::SingleBomb)[dataPtr->index];
			weapon.weaponIns->SetStartPos(dataPtr->pos);
			weapon.weaponIns->SetViewPosCircle();
			weapon.live = true; 
			break;
		}
		case MsgDataBossBombInform::INFORM_TYPE::StalkerBomb:
		{
			auto& weapon = SubObjSerch<GrapeBossWeaponManager>()->GetWeapons(WeaponType::StalkerBomb)[dataPtr->index];
			weapon.weaponIns->SetStartPos(dataPtr->pos);
			weapon.weaponIns->SetViewPosCircle();
			weapon.live = true; 
			break;
		}
		case MsgDataBossBombInform::INFORM_TYPE::RandomBomb:
		{
			auto& weapon = SubObjSerch<GrapeBossWeaponManager>()->GetWeapons(WeaponType::RandomBomb)[dataPtr->index];
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
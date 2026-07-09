#include "GrapeBoss.h"

#include "../../../../Application/Application.h"

#include "../../../../Manager/Net/NetWorkManager.h"
#include "../../../../Manager/Font/FontManager.h"

#include "Weapon/GrapeBossWeaponManager.h"

#include "../../../Common/Collider/LineCollider.h"
#include "../../../Common/Collider/CapsuleCollider.h"
#include "../../../Common/Collider/XZCircleCollider.h"

#include "State/Idle/GrapeBossIdleState.h"
#include "State/Move/GrapeBossMoveState.h"
#include "State/Straight/GrapeBossStraightState.h"
#include "State/Death/GrapeBossDeathState.h"

#include "../../../../Scene/Game/GameScene.h"

#include "../../../../Manager/Effect/EffectManager.h"

#include "../../../UI/CharacterHpUI/CharacterHpUI.h"
#include "../../../UI/HitUI/HitUI.h"


GrapeBoss::GrapeBoss(const std::vector<const Vector3*> playerPos) :
	BossBase(
		"HP",
		"AttackPower",
		"DefensePower",
		"MoveSpeed",
		"Data/Parameter/Character/Boss/Grape/",
		"Grape/GrapeModel",

		playerPos)
{

	coolTime = 120;

	this->operatorSenderId = Net::HOST_SENDER_ID;
	isOwnOperator = true;
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
			// 頭突きへの状態遷移関数のポインタ
			[&]() { ChangeState((int)STATE::ATTACK_A); },
			// スタンプへの状態遷移関数のポインタ
			[&]() { ChangeState((int)STATE::ATTACK_B); },
			// 突進への状態遷移関数のポインタ
			[&]() { ChangeState((int)STATE::ATTACK_C); }
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
			// 角度を戻す
			[&]() { trans.angle.x = 0; },
			// 移動後攻撃に状態遷移関数のポインタ
			[&]() { ChangeState((int)STATE::ATTACK_A); }
		)
	);

	AddState(
		static_cast<int>(STATE::ATTACK_A),
		new GrapeBossStraightState(
			// 自分の状態に遷移する関数
			[&]() { state = static_cast<int>(STATE::ATTACK_A); },
			// 自分の状態かどうかを返す関数
			[&]() { return state == static_cast<int>(STATE::ATTACK_A); },
			// 自分の座標と角度
			trans.pos, trans.angle,
			// 攻撃の種類を情報
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
			[&]() { ChangeState((int)STATE::IDLE); }
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

void GrapeBoss::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other)
{
	BossBase::OnCollision(ownTag, other);
}
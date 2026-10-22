#include "TomatoPlayer.h"

#include "../../../../../Manager/Net/NetWorkManager.h"

#include "../../../../../Scene/SceneManager.h"

#include "TomatoUniqueState/Move/TomatoPlayerMoveState.h"
#include "TomatoUniqueState/Tackle/TomatoPlayerTackleState.h"
#include "TomatoUniqueState/HeadButt/TomatoPlayerHeadButtState.h"
#include "TomatoUniqueState/Stamp/TomatoPlayerStampState.h"
#include "TomatoUniqueState/Damage/TomatoPlayerDamageState.h"
#include "TomatoUniqueState/Death/TomatoPlayerDeathState.h"

#include "../../CommonPlayerState/SimpleAttack/PlayerSimpleAttackCollOperator.h"

#include "../../../UI/HpBarUI.h"
#include "../../../UI/HitUI.h"

#include "../../PlayerUI/PlayerSkillUI.h"
#include "../../PlayerUI/PlayerStaminaUI.h"

TomatoPlayer::TomatoPlayer(MSG_SENDER_ID controlSenderId) :
	PlayerBase(
		controlSenderId,

		"TomatoParameter",
		"PlayerHP",
		"PlayerAttackPower",
		"PlayerDefensePower",
		"PlayerMoveSpeed",

		"Data/Parameter/Character/Player/Tomato/",
		"Tomato/Tomato"
	)
{
}

void TomatoPlayer::PlayerLoad(void)
{
#pragma region 下位オブジェクトの生成

	// 頭突き攻撃の当たり判定を生成
	headButtCollOperatorRef =
		new PlayerSimpleAttackCollOperator(
			100.0f,
			COLLIDER_TAG::PlayerAttack,
			100.0f,
			Vector3::Zonly(100.0f),
			SKILL1_DAMAGE_RATE,
			MsgDataPlayerCollOperator::COLLIDER_TYPE::TomatoPlayerHeadButt,
			trans,
			controlSenderId,
			characterStats
		);
	// 追加
	AddChildActor(headButtCollOperatorRef);


	// 突進攻撃の当たり判定を生成
	tackleCollOperatorRef =
		new PlayerSimpleAttackCollOperator(
			0.0f,
			COLLIDER_TAG::PlayerAttack,
			100.0f,
			Vector3(),
			SKILL2_DAMAGE_RATE,
			MsgDataPlayerCollOperator::COLLIDER_TYPE::TomatoPlayerTackle,
			trans,
			controlSenderId,
			characterStats
		);
	// 追加
	AddChildActor(tackleCollOperatorRef);


	// スタンプ攻撃の当たり判定を生成
	stampCollOperatorRef =
		new PlayerSimpleAttackCollOperator(
			0.0f,
			COLLIDER_TAG::PlayerAttack,
			300.0f,
			Vector3(),
			SKILL3_DAMAGE_RATE,
			MsgDataPlayerCollOperator::COLLIDER_TYPE::TomatoPlayerStamp,
			trans,
			controlSenderId,
			characterStats
		);
	// 追加
	AddChildActor(stampCollOperatorRef);

#pragma endregion 

#pragma region 状態設定

	// 移動状態を追加する
	AddState(
		STATE::MOVE,
		new TomatoPlayerMoveState(
			characterStats,
			1.5f,
			100,
			std::bind(&TomatoPlayer::MoveAccel, this, std::placeholders::_1),
			ACCEL_MAX,
			trans,
			100.0f
		)
	);
	
	// スキル1
	AddState(
		STATE::SKILL_1,
		new TomatoPlayerHeadButtState(
			// クールタイム
			SKILL1_COOL_TIME,
			// 攻撃判定の発生時間
			10.0f, 100.0f,
			HEAD_BUTT_ATTACK_TIME,
			// 頭突き攻撃の当たり判定発生管理用クラス
			*headButtCollOperatorRef,
			trans,
			// 攻撃終了後の状態遷移関数のポインタ (今回は移動状態に遷移するようにする）
			[&]() { ChangeState((int)STATE::MOVE); }
		)
	);

	// スキル2
	AddState(
		STATE::SKILL_2,
		new TomatoPlayerTackleState(
			// 突進攻撃の当たり判定発生管理用クラス
			*tackleCollOperatorRef,
			// 座標 / 角度
			trans,
			// チャージ後の突進の攻撃力を増やす関数のポインタ.
			[&]() { 
				characterStats.attackPower.AddModifier(
					ModifierData(ModifierTypeConversionId(controlSenderId, ModifierType::TackleChargeMaxBuff), 30, 60)
				); 
			},
			[&]() { characterStats.attackPower.DeleteModifier(controlSenderId, ModifierType::TackleChargeMaxBuff); },
			// 攻撃終了後の状態遷移関数のポインタ (今回は移動状態に遷移するようにする）
			[&]() { ChangeState((int)STATE::MOVE); }
		)
	);

	AddState(
		STATE::SKILL_3,
		new TomatoPlayerStampState(
			isGround,
			// スタンプ攻撃の当たり判定発生管理用クラス
			*stampCollOperatorRef,

			std::bind(&TomatoPlayer::MoveAccel, this, std::placeholders::_1),
			ACCEL_MAX,

			velocity,

			[&]() { GetGravityFlg() ? SetGravityFlg(false) : SetGravityFlg(true); },
			[&]() { return  isGround; },
			// 攻撃終了後の状態遷移関数のポインタ (今回は移動状態に遷移するようにする）
			[&]() { ChangeState((int)STATE::MOVE); }
		)
	);

	AddState(
		STATE::DAMAGE,
		new TomatoPlayerDamageState(
			// 自分の状態に関する関数
			[&]() { ChangeState((int)STATE::DAMAGE); },
			// 自分の状態かどうかを返す関数
			[&]() { return state == (int)STATE::DAMAGE; },
			// 座標
			trans.rotation,
			// 無敵時間のセット関数
			[&]() { SetInviCounter(60); },
			// 攻撃終了後の状態遷移関数のポインタ (今回は移動状態に遷移するようにする）
			[&]() { ChangeState((int)STATE::MOVE); }
		)
	);

	// 死亡状態を追加する
	AddState(
		STATE::DEATH,
		new TomatoPlayerDeathState(
			[&]() { ChangeState((int)STATE::DEATH); },
			[&]() { return state == (int)STATE::DEATH; },
			trans.pos, trans.rotation,
			[&]() { PlayerDeathSetting(); },
			[&]() { SetIsDeath(true); },
			[&]() { Net::GetIns().GetConnectStatus().EntryCount() > 1 ? ChangeState((int)STATE::OTHER_WATCH) : ChangeState((int)STATE::MOVE);  }
		)
	);

	// 待機状態/移動状態 -> スキル1 の遷移を登録
	RegisterStateTransition(STATE::IDLE, STATE::SKILL_1);
	RegisterStateTransition(STATE::MOVE, STATE::SKILL_1);

	// 待機状態/移動状態 -> スキル2 の遷移を登録
	RegisterStateTransition(STATE::IDLE, STATE::SKILL_2);
	RegisterStateTransition(STATE::MOVE, STATE::SKILL_2);

	// 待機状態/移動中 -> スキル3の遷移を登録
	RegisterStateTransition(STATE::IDLE, STATE::SKILL_3);
	RegisterStateTransition(STATE::MOVE, STATE::SKILL_3);

#pragma endregion 

#pragma region UIの登録と設定

	// HPUIの座標設定
	int number = 0;
	if (IsOwnControl()) {
		number = 0;
	}
	else {
		// 操作者だけ一番上に表示、それ以外の人のHPは下に描画
		for (int id = 0; id < (int)MSG_SENDER_ID::Max; id++) {
			if (Net::GetIns().GetSenderId() == (MSG_SENDER_ID)id) { continue; }
			number++;
			if (controlSenderId == (MSG_SENDER_ID)id) { break; }
		}
	}

	// HPバーUIの登録
	AddChildActor(
		new HpBarUI(
			PLAYER_UI_IMAGE_FILE_PATH + HP_FRAME_IMAGE_NAME,
			PLAYER_UI_IMAGE_FILE_PATH + HP_IMAGE_NAME,
			PLAYER_UI_IMAGE_FILE_PATH + HP_LOST_IMAGE_NAME,

			HP_FRAME_IMAGE_SIZE,
			HP_BAR_IMAGE_SIZE,

			HP_UI_DRAW_POS[number],

			characterStats,

			PLAYER_UI_IMAGE_FILE_PATH + ICON_BACK_IMAGE_NAME[number],
			PLAYER_UI_IMAGE_FILE_PATH + ICON_CHARA_IMAGE_NAME[(int)SceneManager::GetIns().GetSelectCharaType(controlSenderId)],
			PLAYER_UI_IMAGE_FILE_PATH + ICON_FRAME_IMAGE_NAME[number],

			ICON_IMAGE_SIZE
		)
	);

	// 自分が操作者かどうか
	if (IsOwnControl()) {

		// スタミナUIの登録
		AddChildActor(
			new PlayerStaminaUI(
				GetParameter("Move", "DashStaminaMax"),
				dynamic_cast<TomatoPlayerMoveState*>(&GetStateIns(STATE::MOVE))->GetDashStamina(),
				trans.pos
			)
		);

		// スキル1UIの登録
		AddChildActor(
			new PlayerSkillUI(
				SKILL1_UI_DRAW_POS,
				GetParameterToInt("TripleAttack", "CoolTime"),
				dynamic_cast<TomatoPlayerHeadButtState*>(&GetStateIns(STATE::SKILL_1))->GetCoolTimeCounter(),
				PlayerSkillUI::SKILL_UI_COLOR::Yellow,
				"SkillSlotSimpleAttack"
			)
		);

		// スキル2UIの登録
		AddChildActor(
			new PlayerSkillUI(
				SKILL2_UI_DRAW_POS,
				GetParameterToInt("SimpleAttack", "CoolTime"),
				dynamic_cast<TomatoPlayerTackleState*>(&GetStateIns(STATE::SKILL_2))->GetCoolTimeCounter(),
				PlayerSkillUI::SKILL_UI_COLOR::Red,
				"SkillSlotSimpleAttack"
			)
		);

		// スキル3UIの登録
		AddChildActor(
			new PlayerSkillUI(
				SKILL2_UI_DRAW_POS,
				GetParameterToInt("Dodge", "CoolTime"),
				dynamic_cast<TomatoPlayerStampState*>(&GetStateIns(STATE::SKILL_3))->GetCoolTimeCounter(),
				PlayerSkillUI::SKILL_UI_COLOR::Red,
				"SkillSlotDodge"
			)
		);

	}

#pragma endregion 

	// HPUIの座標設定
	int number = 0;
	if (isOwnOperator) {
		number = 0;
	}
	else {
		// 操作者だけ一番上に表示、それ以外の人のHPは下に描画
		for (int id = 0; id < (int)MSG_SENDER_ID::Max; id++) {
			if (Net::GetIns().GetSenderId() == (MSG_SENDER_ID)id) { continue; }
			number++;
			if (controlSenderId == (MSG_SENDER_ID)id) { break; }
		}
	}

	// HPの登録
	ui_ArrayIns.emplace_back(
		new CharacterHpUI(
			characterStats.hp,
			characterStats.hpMax.Value(),
			HP_FRAME_IMAGE_NAME,
			HP_IMAGE_NAME,
			HP_LOST_IMAGE_NAME,
			"TomatoIcon",
			"P" + std::to_string((int)controlSenderId + 1) + "Frame",
			"P" + std::to_string((int)controlSenderId + 1) + "Back",
			HP_IMAGE_SIZE,
			HP_GAUGE_OFFSET,
			HP_UI_POS[number],
			FILE_PATH_TYPE::PLAYER_HP,
			"TomatoPlayer"
		)
	);

	// このプレイヤーの操作者だけ通す
	if (isOwnOperator)
	{
		// スタミナのUI登録
		ui_ArrayIns.emplace_back(
			new PlayerStaminaUI(
				dynamic_cast<TomatoPlayerMoveState*>(&GetStateIns((int)STATE::MOVE))->GetDashStamina(),
				DASH_STAMINA_MAX
			)
		);
		
		// スキル1UIの登録
		ui_ArrayIns.emplace_back(
			new PlayerSkillUI(
				// 描画座標
				SKILL1_UI_DRAW_POS,
				// クールタイム変数
				dynamic_cast<TomatoPlayerHeadButtState*>(&GetStateIns((int)STATE::SKILL_1))->GetCoolTimeCounter(),
				// クールタイムの最大値
				SKILL1_COOL_TIME,
				// UIの色指定
				PlayerSkillUI::SKILL_UI_COLOR::YELLOW,
				// 描画する画像
				"SkillSlotSimpleAttack"
			)
		);

		// スキル2UIの登録
		ui_ArrayIns.emplace_back(
			new PlayerSkillUI(
				// 描画座標
				SKILL2_UI_DRAW_POS,
				// クールタイム変数
				dynamic_cast<TomatoPlayerTackleState*>(&GetStateIns((int)STATE::SKILL_2))->GetCoolTimeCounter(),
				// クールタイムの最大値
				SKILL2_COOL_TIME,
				// UIの色指定
				PlayerSkillUI::SKILL_UI_COLOR::RED,
				// 描画する画像
				"SkillSlotTomatoTackle",
				true
			)
		);

		// スキル3UIの登録
		ui_ArrayIns.emplace_back(
			new PlayerSkillUI(
				// 描画座標
				SKILL3_UI_DRAW_POS,
				// クールタイム変数
				dynamic_cast<TomatoPlayerStampState*>(&GetStateIns((int)STATE::SKILL_3))->GetCoolTimeCounter(),
				// クールタイムの最大値
				SKILL3_COOL_TIME,
				// UIの色指定
				PlayerSkillUI::SKILL_UI_COLOR::RED,
				// 描画する画像
				"SkillSlotTomatoStamp"
			)
		);
	}

	ui_ArrayIns.emplace_back(new HitUI(trans.pos));
}

void TomatoPlayer::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)
{
	PlayerBase::OnCollision(ownTag, other, result);
}

void TomatoPlayer::ReceptionUpdate(void)
{
	PlayerBase::ReceptionUpdate();

	while (MsgDataPlayerState* dataPtr = Net::GetIns().GetMsgData<MsgDataPlayerState>(controlSenderId)) {
		state = dataPtr->state;

		switch ((STATE)state) {
		case PlayerBase::STATE::SKILL_1: {
			SubActorSerch<TomatoPlayerHeadButtCollOperator>()->ResetIsHit();
			break;
		}
		case PlayerBase::STATE::SKILL_2: {
			tackleAttack->ResetIsHit();
			break;
		}
		case PlayerBase::STATE::SKILL_3: {
			stampAttack->ResetIsHit();
			break;
		}
		case PlayerBase::STATE::DEATH: {
			PlayerDeathSetting();
			break;
		}

		default: { break; }
		}

		delete dataPtr;
	}

	while (auto dataPtr = Net::GetIns().GetMsgData<MsgDataPlayerCollOperator>(controlSenderId)) {

		switch (dataPtr->collKinds) {

		case MsgDataPlayerCollOperator::COLLIDER_TYPE::TomatoPlayerHeadButt: {
			// 三段攻撃
			if (dataPtr->isCollider) { SubActorSerch<TomatoPlayerHeadButtCollOperator>()->CollOn(); }
			else { SubActorSerch<TomatoPlayerHeadButtCollOperator>()->CollOff(); }
			break;
		}

		case MsgDataPlayerCollOperator::COLLIDER_TYPE::TomatoPlayerTackle: {
			// タックル
			if (dataPtr->isCollider) { tackleAttack->CollOn(false); }
			else { tackleAttack->CollOff(false); }
			break;
		}

		case MsgDataPlayerCollOperator::COLLIDER_TYPE::TomatoPlayerStamp: {
			// スタンプ
			if (dataPtr->isCollider) { stampAttack->CollOn(false); }
			else { stampAttack->CollOff(false); }

			break;
		}

		default: { break; }	// 例外
		}

		delete dataPtr;
	}

}

void TomatoPlayer::SendUpdate(void)
{
	PlayerBase::SendUpdate();
}

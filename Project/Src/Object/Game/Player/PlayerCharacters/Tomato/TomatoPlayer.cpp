#include "TomatoPlayer.h"

#include "TomatoUniqueState/Move/TomatoPlayerMoveState.h"
#include "TomatoUniqueState/Tackle/TomatoPlayerTackleState.h"
#include "TomatoUniqueState/HeadButt/TomatoPlayerHeadButtState.h"
#include "TomatoUniqueState/Stamp/TomatoPlayerStampState.h"
#include "TomatoUniqueState/Damage/TomatoPlayerDamageState.h"
#include "TomatoUniqueState/Death/TomatoPlayerDeathState.h"

#include "../../CommonPlayerState/SimpleAttack/PlayerSimpleAttackCollOperator.h"
#include "TomatoUniqueState/HeadButt/TomatoPlayerHeadButtCollOperator.h"


#include "../../../../UI/PlayerStaminaUI/PlayerStaminaUI.h"
#include "../../../../UI/CharacterHpUI/CharacterHpUI.h"
#include "../../../../UI/PlayerSkillUI/PlayerSkillUI.h"
#include "../../../../UI/HitUI/HitUI.h"

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
	subObjArray.emplace_back(
		new TomatoPlayerHeadButtCollOperator(
			COLLIDER_TAG::PlayerAttack,
			SKILL1_DAMAGE_RATE,
			trans.pos, trans.rotation,
			controlSenderId,
			characterStats
		)
	);

	// 突進攻撃の当たり判定を生成
	tackleAttack = new PlayerSimpleAttackCollOperator(0.0f, COLLIDER_TAG::PlayerAttack, 100.0f, Vector3(),
		SKILL2_DAMAGE_RATE, MsgDataPlayerCollOperator::COLLIDER_TYPE::TomatoPlayerTackle,
		trans, controlSenderId, characterStats);
	subObjArray.emplace_back(tackleAttack);

	// スタンプ攻撃の当たり判定を生成
	stampAttack = new PlayerSimpleAttackCollOperator(0.0f, COLLIDER_TAG::PlayerAttack, 300.0f, Vector3(),
		SKILL3_DAMAGE_RATE, MsgDataPlayerCollOperator::COLLIDER_TYPE::TomatoPlayerStamp,
		trans, controlSenderId, characterStats);
	subObjArray.emplace_back(stampAttack);

#pragma endregion 

#pragma region 状態設定

	// 移動状態を追加する
	AddState(
		(int)STATE::MOVE,
		new TomatoPlayerMoveState(
			// 自分の状態に遷移する関数
			[&]() { ChangeState((int)STATE::MOVE); },
			// 自分の状態かどうかを返す関数
			[&]() { return state == (int)STATE::MOVE; },
			// 定数（加算移動量 / 移動量の最大値 / ダッシュの移動量倍率 / スタミナ量 / 加速減衰量）
			DASH_SPEED_RATE, DASH_STAMINA_MAX, ATTENUATION,
			// 参照（移動量 / 横軸加速度の最大値 / 角度）
			accelSum, ACCEL_MAX, trans.rotation, characterStats
		)
	);
	
	// スキル1
	AddState(
		(int)STATE::SKILL_1,
		new TomatoPlayerHeadButtState(
			// 自分の状態に遷移する関数
			[&]() { ChangeState((int)STATE::SKILL_1); },
			// 自分の状態かどうかを返す関数
			[&]() { return state == (int)STATE::SKILL_1; },
			// 頭突き攻撃の当たり判定発生管理用クラス
			*SubActorSerch<TomatoPlayerHeadButtCollOperator>(),
			// クールタイム
			SKILL1_COOL_TIME,
			// 頭突き中の時間 / 頭突きの速度
			HEAD_BUTT_ATTACK_TIME, HEAD_BUTT_SPEED,
			// 座標 / 角度
			trans.pos, trans.rotation,
			// 攻撃終了後の状態遷移関数のポインタ (今回は移動状態に遷移するようにする）
			[&]() { ChangeState((int)STATE::MOVE); }
		)
	);

	// スキル2
	AddState(
		(int)STATE::SKILL_2,
		new TomatoPlayerTackleState(
			// 自分の状態に遷移する関数
			[&]() { ChangeState((int)STATE::SKILL_2); },
			// 自分の状態かどうかを返す関数
			[&]() { return state == (int)STATE::SKILL_2; },
			// 突進攻撃の当たり判定発生管理用クラス
			*tackleAttack,
			// クールタイム
			SKILL2_COOL_TIME,
			// 移動速度 / 回転速度
			TACKLE_SPEED, ROTATION_POW,
			// 座標 / 角度
			trans.pos, trans.rotation,
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
		(int)STATE::SKILL_3,
		new TomatoPlayerStampState(
			// 自分の状態に遷移する関数
			[&]() { ChangeState((int)STATE::SKILL_3); },
			// 自分の状態かどうかを返す関数
			[&]() { return state == (int)STATE::SKILL_3; },
			// スタンプ攻撃の当たり判定発生管理用クラス
			*stampAttack,
			// クールタイム
			SKILL3_COOL_TIME,
			// 移動速度 
			STAMP_JUMP_POWER, STAMP_FALL_SPEED,
			// 座標 / 角度
			trans.pos, trans.rotation, accelSum,
			[&]() { GetGravityFlg() ? SetGravityFlg(false) : SetGravityFlg(true); },
			[&]() { return  isGround; },
			// 攻撃終了後の状態遷移関数のポインタ (今回は移動状態に遷移するようにする）
			[&]() { ChangeState((int)STATE::MOVE); }
		)
	);

	AddState(
		(int)STATE::DAMAGE,
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
		(int)STATE::DEATH,
		new TomatoPlayerDeathState(
			[&]() { ChangeState((int)STATE::DEATH); },
			[&]() { return state == (int)STATE::DEATH; },
			trans.pos, trans.rotation,
			[&]() { PlayerDeathSetting(); },
			[&]() { SetIsDeath(true); },
			[&]() { Net::GetIns().GetConnectStatus().EntryCount() > 1 ? ChangeState((int)STATE::OTHER_WATCH) : ChangeState((int)STATE::MOVE);  }
		)
	);

	// 遷移条件の登録（before = 遷移元)(after = 遷移後）
	auto AddChangeStateCondition = [&](STATE before, STATE after)->void {
		GetStateIns((int)before).AddOtherStateCondition([this, after](void) { GetStateIns((int)after).OwnStateConditionUpdate(); });
		};

	// 移動状態 -> スキル1 の遷移を登録
	AddChangeStateCondition(STATE::MOVE, STATE::SKILL_1);
	// 移動状態 -> スキル2 の遷移を登録
	AddChangeStateCondition(STATE::MOVE, STATE::SKILL_2);
	// 移動状態 -> スキル3 の遷移を登録
	AddChangeStateCondition(STATE::MOVE, STATE::SKILL_3);

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

void TomatoPlayer::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const Vector3& collisionPoint)
{
	PlayerBase::OnCollision(ownTag, other, collisionPoint);
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

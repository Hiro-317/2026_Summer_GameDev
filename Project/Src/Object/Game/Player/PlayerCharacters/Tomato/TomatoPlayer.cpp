#include "TomatoPlayer.h"

#include "../../../../../Manager/TimeScale/TimeScale.h"

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
			// 本体のカプセル半径より一回り大きい球を使用する。
			MODEL_SIZE.x * 0.5f * GetParameter("Collider", "ModelToColliderRate") * 1.2f,
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
			std::bind(&TomatoPlayer::MoveRollingAccel, this, std::placeholders::_1),
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
			HEAD_BUTT_ATTACK_TIME * 0.35f, HEAD_BUTT_ATTACK_TIME * 0.5f,
			HEAD_BUTT_ATTACK_TIME,
			// 頭突き攻撃の当たり判定発生管理用クラス
			*headButtCollOperatorRef,
			trans,
			// PlayerLoad後に共通基盤が設定する、元のモデル座標補正を渡す。
			GetParameterToVector3("Model", "CenterDiff") * GetParameterToVector3("Model", "Scale"),
			HEAD_BUTT_SPEED * HEAD_BUTT_ATTACK_TIME * 0.5f,
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
			// 座標 / 角度と、チャージ中に止める通常移動の慣性。
			trans,
			velocity,
			TACKLE_SPEED,
			ROTATION_POW,
			SKILL2_COOL_TIME,
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
			trans,
			// スタンプ攻撃の当たり判定発生管理用クラス
			*stampCollOperatorRef,
			std::bind(&TomatoPlayer::MoveAccel, this, std::placeholders::_1),
			ACCEL_MAX,
			velocity,
			SKILL3_COOL_TIME,
			STAMP_JUMP_POWER,
			STAMP_FALL_SPEED,
			std::bind(&TomatoPlayer::SetGravityFlg,this,std::placeholders::_1),
			// 攻撃終了後の状態遷移関数のポインタ (今回は移動状態に遷移するようにする）
			[&]() { ChangeState((int)STATE::MOVE); }
		)
	);

	AddState(
		STATE::DAMAGE,
		new TomatoPlayerDamageState(
			// 座標
			trans.rotation,
			velocity,
			// CSVは初期化後に解放されるため、無敵時間をここで取得して保持する。
			[this, damageInviTime = GetParameter("Damage", "DamageInviTime")]() { invincibleCounter = damageInviTime; },
			// 攻撃終了後の状態遷移関数のポインタ (今回は移動状態に遷移するようにする）
			[&]() { ChangeState((int)STATE::MOVE); }
		)
	);

	// 死亡状態を追加する
	AddState(
		STATE::DEATH,
		new TomatoPlayerDeathState(
			trans,
			velocity,
			[this](bool enabled) { SetGravityFlg(enabled); },
			[this](bool enabled) { SetJudgeFlg(enabled); },
			[this]() { isDeath = true; }
		)
	);

	// 待機状態/移動状態 -> スキル1 の遷移を登録
	RegisterStateTransition(STATE::MOVE, STATE::SKILL_1);

	// 待機状態/移動状態 -> スキル2 の遷移を登録
	RegisterStateTransition(STATE::MOVE, STATE::SKILL_2);

	// 待機状態/移動中 -> スキル3の遷移を登録
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
				GetParameterToInt("HeadButt", "CoolTime"),
				dynamic_cast<TomatoPlayerHeadButtState*>(&GetStateIns(STATE::SKILL_1))->GetCoolTimeCounter(),
				PlayerSkillUI::SKILL_UI_COLOR::Yellow,
				"SkillSlotSimpleAttack"
			)
		);

		// スキル2UIの登録
		AddChildActor(
			new PlayerSkillUI(
				SKILL2_UI_DRAW_POS,
				GetParameterToInt("Tackle", "CoolTime"),
				dynamic_cast<TomatoPlayerTackleState*>(&GetStateIns(STATE::SKILL_2))->GetCoolTimeCounter(),
				PlayerSkillUI::SKILL_UI_COLOR::Red,
				"SkillSlotTomatoTackle"
			)
		);

		// スキル3UIの登録
		AddChildActor(
			new PlayerSkillUI(
				SKILL3_UI_DRAW_POS,
				GetParameterToInt("Stamp", "CoolTime"),
				dynamic_cast<TomatoPlayerStampState*>(&GetStateIns(STATE::SKILL_3))->GetCoolTimeCounter(),
				PlayerSkillUI::SKILL_UI_COLOR::Red,
				"SkillSlotTomatoStamp"
			)
		);

	}

#pragma endregion
}

void TomatoPlayer::Update(void)
{
	PlayerBase::Update();

	// 座標の移動後に、このフレームの水平移動量を転がり回転へ反映する。
	if (IsOwnControl() && GetState() == static_cast<int>(STATE::MOVE)) {
		static_cast<TomatoPlayerMoveState&>(GetStateIns(STATE::MOVE)).UpdateRolling();
	}
	if (IsOwnControl() && GetState() == static_cast<int>(STATE::SKILL_3)) {
		static_cast<TomatoPlayerStampState&>(GetStateIns(STATE::SKILL_3)).UpdateColliderPosition();
	}
}

void TomatoPlayer::SubOnGrounded(COLLIDER_TAG ownTag, const ColliderBase& other)
{
    PlayerBase::SubOnGrounded(ownTag, other);
    if (IsOwnControl() && GetState() == static_cast<int>(STATE::SKILL_3)) {
        static_cast<TomatoPlayerStampState&>(GetStateIns(STATE::SKILL_3)).OnGrounded();
    }
}

void TomatoPlayer::MoveRollingAccel(const Vector3& vec)
{
	velocity += (vec * ACCEL_RATE) * TimeScale::Get();
}

void TomatoPlayer::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)
{
	PlayerBase::OnCollision(ownTag, other, result);
}

void TomatoPlayer::ReceptionUpdate(void)
{
	PlayerBase::ReceptionUpdate();

	while (MsgDataPlayerState* dataPtr = Net::GetIns().GetMsgData<MsgDataPlayerState>(controlSenderId)) {

		ChangeState(dataPtr->state);

		delete dataPtr;
	}

	while (auto dataPtr = Net::GetIns().GetMsgData<MsgDataPlayerCollOperator>(controlSenderId)) {

		switch (dataPtr->collKinds) {

		case MsgDataPlayerCollOperator::COLLIDER_TYPE::TomatoPlayerHeadButt: {
			// 通常攻撃
			if (dataPtr->isCollider) { headButtCollOperatorRef->CollOn(false); }
			else { headButtCollOperatorRef->CollOff(false); }
			
			break;
		}

		case MsgDataPlayerCollOperator::COLLIDER_TYPE::TomatoPlayerTackle: {
			// タックル
			if (dataPtr->isCollider) { tackleCollOperatorRef->CollOn(false); }
			else { tackleCollOperatorRef->CollOff(false); }
			break;
		}

		case MsgDataPlayerCollOperator::COLLIDER_TYPE::TomatoPlayerStamp: {
			// スタンプ
			if (dataPtr->isCollider) { stampCollOperatorRef->CollOn(false); }
			else { stampCollOperatorRef->CollOff(false); }

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

#include "PeachPlayer.h"

#include "../../../../../Scene/SceneManager.h"

#include "../../CommonPlayerState/Idle/PlayerIdleState.h"
#include "../../CommonPlayerState/Move/PlayerMoveState.h"

#include "../../CommonPlayerState/SimpleAttack/PlayerSimpleAttackCollOperator.h"
#include "../../CommonPlayerState/SimpleAttack/PlayerSimpleAttackState.h"

#include "../../CommonPlayerState/SingleModifier/PlayerSingleModifierCollOperator.h"
#include "../../CommonPlayerState/SingleModifier/PlayerSingleModifierState.h"

#include "../../CommonPlayerState/Dodge/PlayerDodgeState.h"

#include "../../CommonPlayerState/Damage/PlayerDamageState.h"

#include "../../CommonPlayerState/Death/PlayerDeathState.h"

#include "../../../UI/HpBarUI.h"
#include "../../../UI/HitUI.h"

#include "../../PlayerUI/PlayerSkillUI.h"
#include "../../PlayerUI/PlayerStaminaUI.h"

PeachPlayer::PeachPlayer(MSG_SENDER_ID operatorSenderId) :
	PlayerBase(operatorSenderId,

		"PeachParameter",
		"PlayerHP",
		"PlayerAttackPower",
		"PlayerDefensePower",
		"PlayerMoveSpeed",

		"Data/Parameter/Character/Player/Peach/",
		"Peach/PeachModel"
	),

	punchCollOperatorRef(nullptr),
	healCollOperatorRef(nullptr),
	buffCollOperatorRef(nullptr)
{
}

void PeachPlayer::PlayerLoad(void)
{
	// 影を消す（消さなかったら、変な色合いになるので）
	MV1SetSpcColorScale(trans.model, GetColorF(0.0f, 0.0f, 0.0f, 1.0f));
	MV1SetDifColorScale(trans.model, GetColorF(0.0f, 0.0f, 0.0f, 1.0f));

	// アニメーションコントローラーを生成する
	CreateAnimationController();
	AddInFbxAnimation((int)ANIME_TYPE::MAX, ANIME_SPEED);

#pragma region 下位オブジェクトの生成

	// パンチ用の当たり判定管理クラスを生成
	punchCollOperatorRef =
		new PlayerSimpleAttackCollOperator(
			GetParameter("SimpleAttack", "TargetSerchRange"),
			COLLIDER_TAG::PLAYER_ATTACK,
			GetParameter("SimpleAttack", "CollSize"),
			GetParameterToVector3("SimpleAttack", "CollLocalPos"),
			GetParameterToInt("SimpleAttack", "AttackRatePercent"),
			MsgDataPlayerCollOperator::COLLIDER_TYPE::CommonPlayerSimpleAttack,
			trans,
			controlSenderId,
			characterStats
		);
	// 追加
	AddChildActor(punchCollOperatorRef);

	// 回復用の当たり判定管理クラスを生成
	healCollOperatorRef =
		new PlayerSingleModifierCollOperator(
			COLLIDER_TAG::PLAYER_HEAL,
			controlSenderId,
			MsgDataPlayerCollOperator::COLLIDER_TYPE::PeachPlayerHeal,
			targetPlayerPos,
			100
		);
	// 追加
	AddChildActor(healCollOperatorRef);

	// バフ用の当たり判定管理クラスを生成
	buffCollOperatorRef =
		new PlayerSingleModifierCollOperator(
			COLLIDER_TAG::PLAYER_BUFF,
			controlSenderId,
			MsgDataPlayerCollOperator::COLLIDER_TYPE::PeachPlayerBuff,
			targetPlayerPos,
			80,
			150,
			ModifierType::PeachPlayerSkillBuff
		);
	// 追加
	AddChildActor(buffCollOperatorRef);
	
#pragma endregion 

#pragma region 状態設定

	// 待機状態を追加する
	AddState(
		STATE::IDLE,
		new PlayerIdleState([&]() { AnimePlay(ANIME_TYPE::IDLE); })
	);

	// 移動状態を追加する
	AddState(
		STATE::MOVE,
		new PlayerMoveState(
			// ステータスの参照
			characterStats,
			// ダッシュの移動量倍率
			GetParameter("Move", "DashSpeedRate"),
			// スタミナ量
			(short)GetParameterToInt("Move", "DashStaminaMax"),
			// 移動用の関数
			std::bind(&PeachPlayer::MoveAccel, this, std::placeholders::_1),
			// 横軸加速度の最大値の参照
			ACCEL_MAX,
			// 歩きアニメーション再生
			[&]() { AnimePlay((int)ANIME_TYPE::WALK); },
			// 走りアニメーション再生
			[&]() { AnimePlay((int)ANIME_TYPE::RUN); }
		)
	);

	// 攻撃状態を追加する (パンチ)
	AddState(
		STATE::SKILL_1,
		new PlayerSimpleAttackState(
			// 使用するキー
			KEY_TYPE::PlayerSkill1,
			// クールタイム
			GetParameterToInt("SimpleAttack", "CoolTime"),
			// 攻撃判定発生開始時間
			GetParameter("SimpleAttack", "CollStartTime"),
			// 攻撃判定発生終了時間
			GetParameter("SimpleAttack", "CollEndTime"),
			// 攻撃判定管理
			*punchCollOperatorRef,
			// モデル制御情報の参照
			trans,
			// 移動用の関数
			std::bind(&PeachPlayer::MoveAccel, this, std::placeholders::_1),
			// アニメーション再生
			[&]() { AnimePlay(ANIME_TYPE::PUNCH); },
			// アニメーションの再生割合を取得する関数
			std::bind(&PeachPlayer::GetAnimeRatio, this),
			// 攻撃終了後の状態遷移関数のポインタ (今回は待機状態に遷移するようにする）
			[&]() { ChangeState(STATE::IDLE); }
		)
	);

	// 回復
	AddState(
		STATE::SKILL_2,
		new PlayerSingleModifierState(
			// 使用するキー
			KEY_TYPE::PlayerSkill2,
			// クールタイム
			600,
			// 攻撃判定発生開始時間
			0.3f,
			// 判定管理
			*healCollOperatorRef,
			// アニメーションの再生関数のポインタ
			[&]() { AnimePlay(ANIME_TYPE::HEAL); },
			// アニメーションの再生割合のゲット関数ポインタ
			[&]() { return GetAnimeRatio(); },
			// 攻撃終了後の状態遷移関数のポインタ (今回は移動状態に遷移するようにする）
			[&]() { ChangeState((int)STATE::MOVE); }
		)
	);

	AddState(
		STATE::SKILL_3,
		new PlayerSingleModifierState(
			// 使用するキー
			KEY_TYPE::PlayerSkill3,
			// クールタイム
			600,
			// 攻撃判定発生開始時間
			0.3f,
			// 判定管理
			*healCollOperatorRef,
			// アニメーションの再生関数のポインタ
			[&]() { AnimePlay(ANIME_TYPE::HEAL); },
			// アニメーションの再生割合のゲット関数ポインタ
			[&]() { return GetAnimeRatio(); },
			// 攻撃終了後の状態遷移関数のポインタ (今回は移動状態に遷移するようにする）
			[&]() { ChangeState((int)STATE::MOVE); }
		)
	);

	AddState(
		STATE::DAMAGE,
		new PlayerDamageState(
			// アニメーションの再生
			[&]() { AnimePlay(ANIME_TYPE::DAMAGE); },
			// アニメーションの終了フラグを取得する関数
			std::bind(PeachPlayer::IsAnimeEnd, this),
			// 無敵時間のセット関数
			[&]() { invincibleCounter = 30; },
			// 攻撃終了後の状態遷移関数のポインタ (今回は待機状態に遷移するようにする）
			[&]() { ChangeState(STATE::IDLE); }
		)
	);

	// 死亡状態を追加する
	AddState(
		STATE::DEATH,
		new PlayerDeathState(
			// アニメーションの再生
			[&]() { AnimePlay(ANIME_TYPE::DAMAGE); },
			// アニメーションの終了フラグを取得する関数
			std::bind(PeachPlayer::IsAnimeEnd, this),
			// 死亡演出終了後の処理
			[&]() {
				if (Net::GetIns().GetConnectStatus().EntryCount() > 1) {
					ChangeState((int)STATE::OTHER_WATCH);
				}
			}
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
				dynamic_cast<PlayerMoveState*>(&GetStateIns(STATE::MOVE))->GetDashStamina(),
				trans.pos
			)
		);

		// スキル1UIの登録
		AddChildActor(
			new PlayerSkillUI(
				SKILL1_UI_DRAW_POS,
				GetParameterToInt("TripleAttack", "CoolTime"),
				dynamic_cast<PlayerSimpleAttackState*>(&GetStateIns(STATE::SKILL_1))->GetCoolTimeCounter(),
				PlayerSkillUI::SKILL_UI_COLOR::Yellow,
				"SkillSlotSimpleAttack"
			)
		);

		// スキル2UIの登録
		AddChildActor(
			new PlayerSkillUI(
				SKILL2_UI_DRAW_POS,
				600,
				dynamic_cast<PlayerSingleModifierState*>(&GetStateIns(STATE::SKILL_2))->GetCoolTimeCounter(),
				PlayerSkillUI::SKILL_UI_COLOR::Green,
				"SkillSlotHeal"
			)
		);

		// スキル3UIの登録
		AddChildActor(
			new PlayerSkillUI(
				SKILL3_UI_DRAW_POS,
				600,
				dynamic_cast<PlayerSingleModifierState*>(&GetStateIns(STATE::SKILL_3))->GetCoolTimeCounter(),
				PlayerSkillUI::SKILL_UI_COLOR::Indigo,
				"SkillSlotMoveSpeedBuff"
			)
		);
	}

#pragma endregion 
}

void PeachPlayer::ReceptionUpdate(void)
{
	PlayerBase::ReceptionUpdate();

	while (MsgDataPlayerState* dataPtr = Net::GetIns().GetMsgData<MsgDataPlayerState>(controlSenderId)) {

		ChangeState(dataPtr->state);

		delete dataPtr;
	}

	while (auto dataPtr = Net::GetIns().GetMsgData<MsgDataPlayerCollOperator>(controlSenderId)) {

		switch (dataPtr->collKinds) {

		case MsgDataPlayerCollOperator::COLLIDER_TYPE::CommonPlayerSimpleAttack: {
			// パンチ
			if (dataPtr->isCollider) { punchCollOperatorRef->CollOn(); }
			else { punchCollOperatorRef->CollOff(); }
			break;
		}

		case MsgDataPlayerCollOperator::COLLIDER_TYPE::PeachPlayerHeal: {
			// 回復
			if (dataPtr->isCollider) { healCollOperatorRef->CollOn(); }
			else { healCollOperatorRef->CollOff(); }
			break;
		}

		case MsgDataPlayerCollOperator::COLLIDER_TYPE::PeachPlayerBuff: {
			// バフ
			if (dataPtr->isCollider) { buffCollOperatorRef->CollOn(); }
			else { buffCollOperatorRef->CollOff(); }

			break;
		}

		default: { break; }	// 例外
		}

		delete dataPtr;
	}
}

void PeachPlayer::SendUpdate(void)
{
	PlayerBase::SendUpdate();
}

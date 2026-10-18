#include "OrangePlayer.h"

#include "../../../../../Application/Application.h"

#include "../../../../../Manager/Net/NetWorkManager.h"
#include "../../../../../Manager/Font/FontManager.h"

#include "../../CommonPlayerState/Move/PlayerMoveState.h"

#include "../../CommonPlayerState/TripleAttack/PlayerTripleAttackCollOperator.h"
#include "../../CommonPlayerState/TripleAttack/PlayerTripleAttackState.h"

#include "../../CommonPlayerState/SimpleAttack/PlayerSimpleAttackCollOperator.h"
#include "../../CommonPlayerState/SimpleAttack/PlayerSimpleAttackState.h"

#include "../../CommonPlayerState/Dodge/PlayerDodgeState.h"

#include "../../CommonPlayerState/Damage/PlayerDamageState.h"

#include "../../CommonPlayerState/Death/PlayerDeathState.h"

#include "../../../../UI/PlayerSkillUI/PlayerSkillUI.h"
#include "../../../../UI/PlayerStaminaUI/PlayerStaminaUI.h"
#include "../../../../UI/CharacterHpUI/CharacterHpUI.h"



OrangePlayer::OrangePlayer(MSG_SENDER_ID operatorSenderId) :
	PlayerBase(
		operatorSenderId,

		"OrangeParameter",
		"PlayerHP",
		"PlayerAttackPower",
		"PlayerDefensePower",
		"PlayerMoveSpeed",

		"Data/Parameter/Character/Player/Orange/",
		"Orange/OrangeModel")
{
}


void OrangePlayer::PlayerLoad(void)
{

#pragma region モデル

	// アニメーション～～～～～～～～～～～～～～～～～～～～～～～～～～～～

	// アニメーションコントローラーを生成する
	CreateAnimationController();

	// 各種アニメーションを登録する
	AddInFbxAnimation((int)ANIME_TYPE::MAX, ANIME_SPEED_TABLE);

	// ～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～

#pragma endregion

#pragma region プレイヤーが抱える下位クラスを生成する

	// 三弾攻撃の当たり判定オペレーターを生成する
	AddChildActor(
		new PlayerTripleAttackCollOperator(
			// 探索範囲
			GetParameter("TripleAttack", "TargetSerchRange"),
			// コライダータグ
			COLLIDER_TAG::PLAYER_ATTACK,
			// 攻撃判定の大きさ（球状の半径）
			GetParameter("TripleAttack", "CollSize1"),
			// 攻撃判定発生座標（プレイヤーからの相対座標）
			GetParameterToVector3("TripleAttack", "CollLocalPos"),
			// 各段の攻撃倍率
			{
				(short)GetParameter("TripleAttack", "AttackRatePercent"),
				(short)GetParameter("TripleAttack", "AttackRatePercent"),
				(short)GetParameter("TripleAttack", "AttackRatePercent")
			},
			// モデル制御情報
			trans,
			// 操作権ID
			controlSenderId,
			// ステータス
			characterStats
		)
	);

	// キック用の当たり判定オペレーターを生成する
	AddChildActor(
		new PlayerSimpleAttackCollOperator(
			// 探索範囲
			GetParameter("SimpleAttack", "TargetSerchRange"),
			// コライダータグ
			COLLIDER_TAG::PLAYER_ATTACK,
			// 攻撃判定の大きさ（球状の半径）
			GetParameter("SimpleAttack", "CollSize"),
			// 攻撃判定発生座標（プレイヤーからの相対座標）
			GetParameterToVector3("SimpleAttack", "CollLocalPos"),
			// 攻撃倍率
			GetParameterToInt("SimpleAttack", "AttackRatePercent"),
			// モデル制御情報の参照
			trans,
			// 操作権IDの参照
			controlSenderId,
			// ステータスの参照
			characterStats
		)
	);

#pragma endregion


#pragma region 状態設定

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
			std::bind(&OrangePlayer::MoveAccel,this,std::placeholders::_1),
			// 横軸加速度の最大値の参照
			ACCEL_MAX,
			// 歩きアニメーション再生
			[&]() { AnimePlay((int)ANIME_TYPE::WALK); },
			// 走りアニメーション再生
			[&]() { AnimePlay((int)ANIME_TYPE::RUN); }
		)
	);

	// 三段攻撃状態を追加する
	AddState(
		STATE::SKILL_1,	
		new PlayerTripleAttackState(
			KEY_TYPE::PlayerSkill1,
			GetParameterToInt("TripleAttack", "CoolTime"),
			GetParameterToInt("TripleAttack", "AttackNextStageContinueTime"),
			{
				GetParameter("TripleAttack","CollStartTime1"),
				GetParameter("TripleAttack","CollStartTime2"),
				GetParameter("TripleAttack","CollStartTime3")
			},
			{
				GetParameter("TripleAttack","CollEndTime1"),
				GetParameter("TripleAttack","CollEndTime2"),
				GetParameter("TripleAttack","CollEndTime3"),
			},
			
			,
			// モデル制御情報の参照
			trans,
			// 各段のアニメーション再生
			std::bind(&OrangePlayer::MoveAccel, this, std::placeholders::_1),
			{
				[&]() { AnimePlay((int)ANIME_TYPE::PUNCH1); },
				[&]() { AnimePlay((int)ANIME_TYPE::PUNCH2); },
				[&]() { AnimePlay((int)ANIME_TYPE::PUNCH3); }
			},
			// アニメーションの再生割合を取得する関数
			std::bind(&OrangePlayer::GetAnimeRatio,this),
			// 攻撃終了後の状態遷移関数のポインタ (今回は待機状態に遷移するようにする）
			[&]() { ChangeState(STATE::IDLE); }
		)
	);

	// 攻撃状態を追加する (キック)
	AddState(
		(int)STATE::SKILL_2,
		new PlayerSimpleAttackState(
			// 自分の状態に遷移する関数
			[&]() { ChangeState((int)STATE::SKILL_2); },
			// 自分の状態かどうかを返す関数
			[&]() { return state == (int)STATE::SKILL_2; },
			// 定数（使用するキー / クールタイム / 攻撃の判定を発生させる 開始 / 終了 時間（アニメーションの再生割合）/ 攻撃中の移動速度）
			KEY_TYPE::PLAYER_SKILL_2, SKILL_2_COOL_TIME, SKILL_2_COLL_START_TIME, SKILL_2_COLL_END_TIME, SKILL_2_ATTACK_MOVE_SPEED,
			// 当たり判定のオペレーター
			*SubObjSerch<PlayerSimpleAttackCollOperator>(),
			// 座標 / 角度
			trans.pos, trans.angle,
			// アニメーションの再生関数のポインタ
			[&]() { AnimePlay((int)ANIME_TYPE::KICK, false); },
			// アニメーションの再生割合を取得する関数のポインタ / アニメーションの終了フラグを取得する関数のポインタ
			[&]() { return GetAnimeRatio(); }, [&]() { return IsAnimeEnd(); },
			// 攻撃終了後の状態遷移関数のポインタ (今回は移動状態に遷移するようにする）
			[&]() { ChangeState((int)STATE::MOVE); }
		)
	);

	AddState(
		(int)STATE::SKILL_3,
		new PlayerDodgeState(
			// 自分の状態に関する関数
			[&]() { ChangeState((int)STATE::SKILL_3); },
			// 自分の状態かどうかを返す関数
			[&]() { return state == (int)STATE::SKILL_3; },
			// 定数（使用するキー / クールタイム / 回避中の移動速度 / 無敵時間の 開始 / 終了時間（アニメーションの再生割合
			KEY_TYPE::PLAYER_SKILL_3, SKILL_3_COOL_TIME, SKILL_3_MOVE_SPEED,
			SKILL_3_INVI_START_TIME, SKILL_3_INVI_END_TIME,
			// 座標 / 角度
			trans.pos, trans.angle,
			// アニメーションの再生関数のポインタ
			[&]() { AnimePlay((int)ANIME_TYPE::DODGE, false); },
			// アニメーションの再生割合を取得する関数のポインタ / アニメーションの終了フラグを取得する関数のポインタ
			[&]() { return GetAnimeRatio(); }, [&]() { return IsAnimeEnd(); },
			// 無敵時間のセット関数
			std::bind(&OrangePlayer::SetInviCounter, this, std::placeholders::_1),
			// 攻撃終了後の状態遷移関数のポインタ (今回は移動状態に遷移するようにする）
			[&]() { ChangeState((int)STATE::MOVE); }
		)
	);

	AddState(
		(int)STATE::DAMAGE,
		new PlayerDamageState(
			// 自分の状態に関する関数
			[&]() { ChangeState((int)STATE::DAMAGE); },
			// 自分の状態かどうかを返す関数
			[&]() { return state == (int)STATE::DAMAGE; },
			// アニメーションの再生関数のポインタ
			[&]() { AnimePlay((int)ANIME_TYPE::DAMAGE, false); },
			// アニメーションの終了フラグを取得する関数のポインタ
			[&]() { return IsAnimeEnd(); },
			// 無敵時間のセット関数
			[&]() { SetInviCounter(DODGE_INVI_TIME); },
			// 攻撃終了後の状態遷移関数のポインタ (今回は移動状態に遷移するようにする）
			[&]() { ChangeState((int)STATE::MOVE); }
		)
	);

	// 死亡状態を追加する
	AddState(
		(int)STATE::DEATH,
		new PlayerDeathState(
			// 自分の状態に関する関数
			[&]() { ChangeState((int)STATE::DEATH); },
			// 自分の状態かどうかを返す関数
			[&]() { return state == (int)STATE::DEATH; },
			// 座標 / 角度
			trans.pos, trans.angle,
			// アニメーション終了判定ゲット関数のポインタ
			[&]() { return IsAnimeEnd(); },
			// 死亡アニメーション再生関数のポインタ
			[&]() { AnimePlay((int)ANIME_TYPE::DEATH, false); },
			// 死亡後の設定関数のポインタ
			[&]() { PlayerDeathSetting(); },
			// 攻撃終了後の状態遷移関数のポインタ
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
	// 移動中 -> スキル3の遷移を登録
	AddChangeStateCondition(STATE::MOVE, STATE::SKILL_3);

#pragma endregion

#pragma region UIの登録と設定

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
			if (operatorSenderId == (MSG_SENDER_ID)id) { break; }
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
			"OrangeIcon",
			"P" + std::to_string((int)operatorSenderId + 1) + "Frame",
			"P" + std::to_string((int)operatorSenderId + 1) + "Back",
			HP_IMAGE_SIZE,
			HP_GAUGE_OFFSET,
			HP_UI_POS[number],
			FILE_PATH_TYPE::PLAYER_HP,
			""
		)
	);

	// 自分が操作者かどうか
	if (isOwnOperator) {

		// スタミナのUI登録
		ui_ArrayIns.emplace_back(
			new PlayerStaminaUI(
				dynamic_cast<PlayerMoveState*>(&GetStateIns((int)STATE::MOVE))->GetDashStamina(),
				DASH_STAMINA_MAX
			)
		);

		// スキル1のUI
		ui_ArrayIns.emplace_back(
			new PlayerSkillUI(
				SKILL1_UI_DRAW_POS,
				dynamic_cast<PlayerTripleAttackState*>(&GetStateIns((int)STATE::SKILL_1))->GetCoolTimeCounter(),
				SKILL_1_COOL_TIME,
				PlayerSkillUI::SKILL_UI_COLOR::YELLOW,
				"SkillSlotTripleAttack"
			)
		);

		// スキル2のUI
		ui_ArrayIns.emplace_back(
			new PlayerSkillUI(
				SKILL2_UI_DRAW_POS,
				dynamic_cast<PlayerSimpleAttackState*>(&GetStateIns((int)STATE::SKILL_2))->GetCoolTimeCounter(),
				SKILL_2_COOL_TIME,
				PlayerSkillUI::SKILL_UI_COLOR::RED,
				"SkillSlotSimpleAttack"
			)
		);

		// スキル3のUI
		ui_ArrayIns.emplace_back(
			new PlayerSkillUI(
				SKILL3_UI_DRAW_POS,
				dynamic_cast<PlayerDodgeState*>(&GetStateIns((int)STATE::SKILL_3))->GetCoolTimeCounter(),
				SKILL_3_COOL_TIME,
				PlayerSkillUI::SKILL_UI_COLOR::BLUE,
				"SkillSlotDodge"
			)
		);
	}

	ui_ArrayIns.emplace_back(new HitUI(trans.pos));

#pragma endregion 
}

void OrangePlayer::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)
{
	// 回避時の当たり判定処理(回避成功したら、ダメージ処理をスルーする)
	if (PlayerBase::DodgeOnCollision(STATE::SKILL_3, other)) { return; }

	// 敵の攻撃を受けた時のダメージ処理
	PlayerBase::OnCollision(ownTag, other, result);
}

void OrangePlayer::ReceptionUpdate(void)
{
	PlayerBase::ReceptionUpdate();

	while (MsgDataPlayerState* dataPtr = Net::GetIns().GetMsgData<MsgDataPlayerState>(operatorSenderId)) {
		state = dataPtr->state;

		switch ((STATE)state) {
		case PlayerBase::STATE::SKILL_1: {
			SubObjSerch<PlayerTripleAttackCollOperator>()->ResetIsHit();
			break;
		}
		case PlayerBase::STATE::SKILL_2: {
			SubObjSerch<PlayerSimpleAttackCollOperator>()->ResetIsHit();
			break;
		}
		case PlayerBase::STATE::SKILL_3: {
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

	while (auto dataPtr = Net::GetIns().GetMsgData<MsgDataPlayerCollOperator>(operatorSenderId)) {
		
		switch (dataPtr->collKinds) {

		case MsgDataPlayerCollOperator::COLLIDER_TYPE::CommonPlayerTripleAttack_1:
		case MsgDataPlayerCollOperator::COLLIDER_TYPE::CommonPlayerTripleAttack_2:
		case MsgDataPlayerCollOperator::COLLIDER_TYPE::CommonPlayerTripleAttack_3: {
			// 三段攻撃
			if (dataPtr->isCollider) { SubObjSerch<PlayerTripleAttackCollOperator>()->CollOn((PLAYER_TRIPLE_ATTACK_STAGE)dataPtr->collKinds); }
			else { SubObjSerch<PlayerTripleAttackCollOperator>()->CollOff((PLAYER_TRIPLE_ATTACK_STAGE)dataPtr->collKinds); }
			break;
		}

		case MsgDataPlayerCollOperator::COLLIDER_TYPE::CommonPlayerSimpleAttack: {
			// キック
			if (dataPtr->isCollider) { SubObjSerch<PlayerSimpleAttackCollOperator>()->CollOn(); }
			else { SubObjSerch<PlayerSimpleAttackCollOperator>()->CollOff(); }
			break;
		}

		default: { break; }	// 例外
		}

		delete dataPtr;
	}
}

void OrangePlayer::SendUpdate(void)
{
	PlayerBase::SendUpdate();
}




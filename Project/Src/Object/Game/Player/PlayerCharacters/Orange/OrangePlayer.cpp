#include "OrangePlayer.h"

#include "../../../../../Application/Application.h"

#include "../../../../../Manager/Net/NetWorkManager.h"
#include "../../../../../Manager/Font/FontManager.h"

#include "../../../../../Scene/SceneManager.h"

#include "../../../../Common/Collider/ColliderBase.h"

#include "../../CommonPlayerState/Idle/PlayerIdleState.h"
#include "../../CommonPlayerState/Move/PlayerMoveState.h"

#include "../../CommonPlayerState/TripleAttack/PlayerTripleAttackCollOperator.h"
#include "../../CommonPlayerState/TripleAttack/PlayerTripleAttackState.h"

#include "../../CommonPlayerState/SimpleAttack/PlayerSimpleAttackCollOperator.h"
#include "../../CommonPlayerState/SimpleAttack/PlayerSimpleAttackState.h"

#include "../../CommonPlayerState/Dodge/PlayerDodgeState.h"

#include "../../CommonPlayerState/Damage/PlayerDamageState.h"

#include "../../CommonPlayerState/Death/PlayerDeathState.h"

#include "../../../UI/HpBarUI.h"
#include "../../../UI/HitUI.h"

#include "../../PlayerUI/PlayerSkillUI.h"
#include "../../PlayerUI/PlayerStaminaUI.h"

OrangePlayer::OrangePlayer(MSG_SENDER_ID controlSenderId) :
	PlayerBase(
		controlSenderId,

		"OrangeParameter",
		"PlayerHP",
		"PlayerAttackPower",
		"PlayerDefensePower",
		"PlayerMoveSpeed",

		"Data/Parameter/Character/Player/Orange/",
		"Orange/OrangeModel"
	),

	tripleAttackCollOperatorRef(nullptr),
	kickCollOperatorRef(nullptr),

	isInvincible(false),

	hitUiRef(nullptr)
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
	tripleAttackCollOperatorRef =
		new PlayerTripleAttackCollOperator(
			// 探索範囲
			GetParameter("TripleAttack", "TargetSerchRange"),
			// コライダータグ
			COLLIDER_TAG::PlayerAttack,
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
			// 各段の送信コライダータイプ
			{
				MsgDataPlayerCollOperator::COLLIDER_TYPE::CommonPlayerTripleAttack_1,
				MsgDataPlayerCollOperator::COLLIDER_TYPE::CommonPlayerTripleAttack_2,
				MsgDataPlayerCollOperator::COLLIDER_TYPE::CommonPlayerTripleAttack_3
			},
			// モデル制御情報
			trans,
			// 操作権ID
			controlSenderId,
			// ステータス
			characterStats
		);
	// 下位クラスに追加
	AddChildActor(tripleAttackCollOperatorRef);


	// キック用の当たり判定オペレーターを生成する
	kickCollOperatorRef =
		new PlayerSimpleAttackCollOperator(
			// 探索範囲
			GetParameter("SimpleAttack", "TargetSerchRange"),
			// コライダータグ
			COLLIDER_TAG::PlayerAttack,
			// 攻撃判定の大きさ（球状の半径）
			GetParameter("SimpleAttack", "CollSize"),
			// 攻撃判定発生座標（プレイヤーからの相対座標）
			GetParameterToVector3("SimpleAttack", "CollLocalPos"),
			// 攻撃倍率
			GetParameterToInt("SimpleAttack", "AttackRatePercent"),
			// 送信コライダータイプ
			MsgDataPlayerCollOperator::COLLIDER_TYPE::CommonPlayerSimpleAttack,
			// モデル制御情報の参照
			trans,
			// 操作権IDの参照
			controlSenderId,
			// ステータスの参照
			characterStats
		);
	// 下位クラスに追加
	AddChildActor(kickCollOperatorRef);

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
			std::bind(&OrangePlayer::MoveAccel,this,std::placeholders::_1),
			// 横軸加速度の最大値の参照
			ACCEL_MAX,
			// 歩きアニメーション再生
			[&]() { AnimePlay(ANIME_TYPE::WALK); },
			// 走りアニメーション再生
			[&]() { AnimePlay(ANIME_TYPE::RUN); }
		)
	);

	// 三段攻撃状態を追加する
	AddState(
		STATE::SKILL_1,
		new PlayerTripleAttackState(
			// 使用するキー
			KEY_TYPE::PlayerSkill1,
			// クールタイム
			GetParameterToInt("TripleAttack", "CoolTime"),
			// 次段につながるまでの猶予時間
			GetParameterToInt("TripleAttack", "AttackNextStageContinueTime"),
			// 各段の攻撃判定発生開始時間
			{
				GetParameter("TripleAttack","CollStartTime1"),
				GetParameter("TripleAttack","CollStartTime2"),
				GetParameter("TripleAttack","CollStartTime3")
			},
			// 各段の攻撃判定発生終了時間
			{
				GetParameter("TripleAttack","CollEndTime1"),
				GetParameter("TripleAttack","CollEndTime2"),
				GetParameter("TripleAttack","CollEndTime3"),
			},
			// 攻撃判定管理
			* tripleAttackCollOperatorRef,
			// モデル制御情報の参照
			trans,
			// 各段のアニメーション再生
			std::bind(&OrangePlayer::MoveAccel, this, std::placeholders::_1),
			{
				[&]() { AnimePlay(ANIME_TYPE::PUNCH1); },
				[&]() { AnimePlay(ANIME_TYPE::PUNCH2); },
				[&]() { AnimePlay(ANIME_TYPE::PUNCH3); }
			},
			// アニメーションの再生割合を取得する関数
			std::bind(&OrangePlayer::GetAnimeRatio, this),
			// 攻撃終了後の状態遷移関数のポインタ (今回は待機状態に遷移するようにする）
			[&]() { ChangeState(STATE::IDLE); }
		)
	);

	// 攻撃状態を追加する (キック)
	AddState(
		STATE::SKILL_2,
		new PlayerSimpleAttackState(
			// 使用するキー
			KEY_TYPE::PlayerSkill2,
			// クールタイム
			GetParameterToInt("SimpleAttack", "CoolTime"),
			// 攻撃判定発生開始時間
			GetParameter("SimpleAttack", "CollStartTime"),
			// 攻撃判定発生終了時間
			GetParameter("SimpleAttack", "CollEndTime"),
			// 攻撃判定管理
			*kickCollOperatorRef,
			// モデル制御情報の参照
			trans,
			// 移動用の関数
			std::bind(&OrangePlayer::MoveAccel, this, std::placeholders::_1),
			// アニメーション再生
			[&]() { AnimePlay(ANIME_TYPE::KICK); },
			// アニメーションの再生割合を取得する関数
			std::bind(&OrangePlayer::GetAnimeRatio, this),
			// 攻撃終了後の状態遷移関数のポインタ (今回は待機状態に遷移するようにする）
			[&]() { ChangeState(STATE::IDLE); }
		)
	);

	AddState(
		STATE::SKILL_3,
		new PlayerDodgeState(
			// 使用するキー
			KEY_TYPE::PlayerSkill3,
			// クールタイム
			GetParameterToInt("Dodge", "CoolTime"),
			// 無敵判定発生開始時間
			GetParameter("Dodge", "InviStartTime"),
			// 無敵判定発生終了時間
			GetParameter("Dodge", "InviEndTime"),
			// 回転情報の参照
			trans.rotation,
			// 移動用の関数
			std::bind(&OrangePlayer::MoveAccel, this, std::placeholders::_1),
			// 無敵判定操作
			isInvincible,
			// アニメーション再生
			[&]() { AnimePlay(ANIME_TYPE::DODGE); },
			// アニメーションの再生割合を取得する関数
			std::bind(&OrangePlayer::GetAnimeRatio, this),
			// 攻撃終了後の状態遷移関数のポインタ (今回は待機状態に遷移するようにする）
			[&]() { ChangeState(STATE::IDLE); }
		)
	);

	AddState(
		STATE::DAMAGE,
		new PlayerDamageState(
			// アニメーションの再生
			[&]() { AnimePlay(ANIME_TYPE::DAMAGE); },
			// アニメーションの終了フラグを取得する関数
			std::bind(OrangePlayer::IsAnimeEnd, this),
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
			std::bind(OrangePlayer::IsAnimeEnd, this),
			// 死亡演出終了後の処理
			[&]() { isDeath = true; }
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
				dynamic_cast<PlayerTripleAttackState*>(&GetStateIns(STATE::SKILL_1))->GetCoolTimeCounter(),
				PlayerSkillUI::SKILL_UI_COLOR::Yellow,
				"SkillSlotTripleAttack"
			)
		);

		// スキル2UIの登録
		AddChildActor(
			new PlayerSkillUI(
				SKILL2_UI_DRAW_POS,
				GetParameterToInt("SimpleAttack", "CoolTime"),
				dynamic_cast<PlayerSimpleAttackState*>(&GetStateIns(STATE::SKILL_2))->GetCoolTimeCounter(),
				PlayerSkillUI::SKILL_UI_COLOR::Red,
				"SkillSlotSimpleAttack"
			)
		);

		// スキル3UIの登録
		AddChildActor(
			new PlayerSkillUI(
				SKILL2_UI_DRAW_POS,
				GetParameterToInt("Dodge", "CoolTime"),
				dynamic_cast<PlayerDodgeState*>(&GetStateIns(STATE::SKILL_3))->GetCoolTimeCounter(),
				PlayerSkillUI::SKILL_UI_COLOR::Blue,
				"SkillSlotDodge"
			)
		);

		// ヒットUIの生成 / 登録
		hitUiRef = new HitUI();
		AddChildActor(hitUiRef);
	}

#pragma endregion 
}

void OrangePlayer::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)
{
	// 判定そのものはホストがすべて請け負う
	if (!Net::GetIns().IsHost()) { return; }

	// 回避時の特殊無敵判定
	if (isInvincible) {
		// ダメージが発生するコライダーを検出
		if (other.GetTag() == COLLIDER_TAG::BossAttack) {

			// 回避成功のエフェクトを発生
			hitUiRef->MissSetting(result.point);

			// 回避成功を通知
			Net::GetIns().Send(MsgDataPlayerMissNotice(result.point), controlSenderId, controlSenderId);

			// 通常の無敵判定を発生させる
			invincibleCounter = 30;
		}
	}

	// 敵の攻撃を受けた時のダメージ処理
	PlayerBase::OnCollision(ownTag, other, result);
}

void OrangePlayer::ReceptionUpdate(void)
{
	PlayerBase::ReceptionUpdate();

	while (MsgDataPlayerState* dataPtr = Net::GetIns().GetMsgData<MsgDataPlayerState>(controlSenderId)) {

		ChangeState(dataPtr->state);

		delete dataPtr;
	}

	while (auto dataPtr = Net::GetIns().GetMsgData<MsgDataPlayerCollOperator>(controlSenderId)) {
		
		switch (dataPtr->collKinds) {

		case MsgDataPlayerCollOperator::COLLIDER_TYPE::CommonPlayerTripleAttack_1: {
			// 三段攻撃（1段目）
			if (dataPtr->isCollider) { tripleAttackCollOperatorRef->CollOn(PLAYER_TRIPLE_ATTACK_STAGE::First); }
			else { tripleAttackCollOperatorRef->CollOff(PLAYER_TRIPLE_ATTACK_STAGE::First); }
			break;
		}
		case MsgDataPlayerCollOperator::COLLIDER_TYPE::CommonPlayerTripleAttack_2: {
			// 三段攻撃（2段目）
			if (dataPtr->isCollider) { tripleAttackCollOperatorRef->CollOn(PLAYER_TRIPLE_ATTACK_STAGE::Second); }
			else { tripleAttackCollOperatorRef->CollOff(PLAYER_TRIPLE_ATTACK_STAGE::Second); }
			break;
		}
		case MsgDataPlayerCollOperator::COLLIDER_TYPE::CommonPlayerTripleAttack_3: {
			// 三段攻撃（3段目）
			if (dataPtr->isCollider) { tripleAttackCollOperatorRef->CollOn(PLAYER_TRIPLE_ATTACK_STAGE::Third); }
			else { tripleAttackCollOperatorRef->CollOff(PLAYER_TRIPLE_ATTACK_STAGE::Third); }
			break;
		}

		case MsgDataPlayerCollOperator::COLLIDER_TYPE::CommonPlayerSimpleAttack: {
			// キック
			if (dataPtr->isCollider) { kickCollOperatorRef->CollOn(); }
			else { kickCollOperatorRef->CollOff(); }
			break;
		}

		default: { break; }	// 例外
		}

		delete dataPtr;
	}

	// 回避した時の「ミス！」表示
	while (auto dataPtr = Net::GetIns().GetMsgData<MsgDataPlayerMissNotice>(controlSenderId)) {

		hitUiRef->MissSetting(dataPtr->point);

		delete dataPtr;
	}
}

void OrangePlayer::SendUpdate(void)
{
	PlayerBase::SendUpdate();
}
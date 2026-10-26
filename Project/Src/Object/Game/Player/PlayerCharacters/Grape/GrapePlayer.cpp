#include "GrapePlayer.h"

#include "../../../../../Manager/Net/NetWorkManager.h"

#include "../../../../../Scene/SceneManager.h"

#include "../../CommonPlayerState/Idle/PlayerIdleState.h"
#include "../../CommonPlayerState/Move/PlayerMoveState.h"
#include "../../CommonPlayerState/Damage/PlayerDamageState.h"
#include "../../CommonPlayerState/Death/PlayerDeathState.h"
#include "GrapeUniqueState/Bomb/GrapePlayerBombState.h"
#include "GrapeUniqueState/Shot/GrapePlayerShotState.h"
#include "GrapeUniqueState/Throw/GrapePlayerThrowState.h"

#include "GrapeUniqueState/Bomb/GrapePlayerWeaponBomb.h"
#include "GrapeUniqueState/Shot/GrapePlayerWeaponShot.h"
#include "GrapeUniqueState/Throw/GrapePlayerWeaponThrow.h"

#include "../../../UI/HpBarUI.h"

#include "../../PlayerUI/PlayerSkillUI.h"
#include "../../PlayerUI/PlayerStaminaUI.h"

GrapePlayer::GrapePlayer(MSG_SENDER_ID controlSenderId) :
	PlayerBase(controlSenderId,

		"GrapeParameter",
		"PlayerHP",
		"PlayerAttackPower",
		"PlayerDefensePower",
		"PlayerMoveSpeed",

		"Data/Parameter/Character/Player/Grape/",
		"Grape/GrapeModel"
	)
{
}

void GrapePlayer::PlayerLoad(void)
{
	// アニメーションコントローラーを生成する
	CreateAnimationController();

	// アニメーションの読み込み
	AddInFbxAnimation((int)ANIME_TYPE::MAX, ANIME_SPEED);

#pragma region 下位オブジェクトの生成

	weaponShotRef = new GrapePlayerWeaponShot(
		GetParameter("GrapeShot", "AttackRate"),
		characterStats,
		trans,
		controlSenderId
	);
	AddChildActor(weaponShotRef);

	weaponThrowRef = new GrapePlayerWeaponThrow(
		GetParameter("GrapeThrowBomb", "AttackRate"),
		characterStats,
		trans,
		controlSenderId
	);
	AddChildActor(weaponThrowRef);

	weaponBombRef = new GrapePlayerWeaponBomb(
		GetParameter("GrapeBomb", "AttackRate"),
		characterStats,
		GetParameter("GrapeBomb", "CountTime"),
		trans,
		controlSenderId
	);
	AddChildActor(weaponBombRef);


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
			std::bind(&GrapePlayer::MoveAccel, this, std::placeholders::_1),
			// 横軸加速度の最大値の参照
			ACCEL_MAX,
			// 歩きアニメーション再生
			[&]() { AnimePlay(ANIME_TYPE::WALK); },
			// 走りアニメーション再生
			[&]() { AnimePlay(ANIME_TYPE::RUN); }
		)
	);

	AddState(
		STATE::SKILL_1,
		new GrapePlayerShotState(
			GetParameter("GrapeShot", "CoolTime"),
			*weaponShotRef,
			trans,
			bossPos,
			[&]() { AnimePlay(ANIME_TYPE::THROW); },
			[&]() { return GetAnimeRatio(); },
			[&]() { ChangeState(STATE::IDLE); }
		)
	);

	AddState(
		STATE::SKILL_2,
		new GrapePlayerThrowState(
			GetParameter("GrapeThrowBomb", "CoolTime"),
			*weaponThrowRef,
			trans,
			bossPos,
			[&]() { AnimePlay(ANIME_TYPE::THROW); },
			[&]() { return GetAnimeRatio(); },
			[&]() { ChangeState(STATE::IDLE); }
		)
	);

	// 設置型爆弾
	AddState(
		STATE::SKILL_3,
		new GrapePlayerBombState(
			GetParameter("GrapeBomb", "CoolTime"),
			*weaponBombRef,
			trans,
			[&]() { AnimePlay(ANIME_TYPE::TOSS); },
			[&]() { return GetAnimeRatio(); },
			[&]() { ChangeState(STATE::IDLE); }
		)
	);


	AddState(
		STATE::DAMAGE,
		new PlayerDamageState(
			// アニメーションの再生
			[&]() { AnimePlay(ANIME_TYPE::DAMAGE); },
			// アニメーションの終了フラグを取得する関数
			std::bind(&GrapePlayer::IsAnimeEnd, this),
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
			std::bind(&GrapePlayer::IsAnimeEnd, this),
			// 死亡演出終了後の処理
			[&]() {isDeath = true; }
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
				GetParameter("GrapeShot", "CoolTime"),
				dynamic_cast<GrapePlayerShotState*>(&GetStateIns(STATE::SKILL_1))->GetCoolTimeCounter(),
				PlayerSkillUI::SKILL_UI_COLOR::Yellow,
				"SkillSlotShot"
			)
		);

		// スキル2UIの登録
		AddChildActor(
			new PlayerSkillUI(
				SKILL2_UI_DRAW_POS,
				GetParameter("GrapeThrowBomb", "CoolTime"),
				dynamic_cast<GrapePlayerThrowState*>(&GetStateIns(STATE::SKILL_2))->GetCoolTimeCounter(),
				PlayerSkillUI::SKILL_UI_COLOR::Purple,
				"SkillSlotGrapeBombThrow"
			)
		);

		// スキル3UIの登録
		AddChildActor(
			new PlayerSkillUI(
				SKILL2_UI_DRAW_POS,
				GetParameter("GrapeBomb", "CoolTime"),
				dynamic_cast<GrapePlayerBombState*>(&GetStateIns(STATE::SKILL_3))->GetCoolTimeCounter(),
				PlayerSkillUI::SKILL_UI_COLOR::Purple,
				"SkillSlotGrapeBombPut"
			)
		);

	}

#pragma endregion
}

void GrapePlayer::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)
{
	// 敵の攻撃を受けた時のダメージ処理
	PlayerBase::OnCollision(ownTag, other, result);
}

void GrapePlayer::ReceptionUpdate(void)
{
	PlayerBase::ReceptionUpdate();

	while (MsgDataPlayerState* dataPtr = Net::GetIns().GetMsgData<MsgDataPlayerState>(controlSenderId)) {

		ChangeState(dataPtr->state);

		delete dataPtr;
	}
}

void GrapePlayer::SendUpdate(void)
{
	PlayerBase::SendUpdate();
}
#include "OrangePlayer.h"

#include "../../../../Application/Application.h"

#include "../../../../Manager/Net/NetWorkManager.h"
#include "../../../../Manager/Font/FontManager.h"
#include "../../../../Manager/Camera/Camera.h"

#include "../CommonPlayerState/Move/PlayerMoveState.h"
#include "../CommonPlayerState/TripleAttack/PlayerTripleAttackState.h"
#include "../CommonPlayerState/SimpleAttack/PlayerSimpleAttackState.h"
#include "../CommonPlayerState/Dodge/PlayerDodgeState.h"
#include "../CommonPlayerState/Damage/PlayerDamageState.h"
#include "../CommonPlayerState/Death/PlayerDeathState.h"

#include "../../../UI/PlayerSkillUI/PlayerSkillUI.h"
#include "../../../UI/PlayerStaminaUI/PlayerStaminaUI.h"
#include "../../../UI/CharacterHpUI/CharacterHpUI.h"



OrangePlayer::OrangePlayer(MSG_SENDER_ID operatorSenderId) :
	PlayerBase(
		"OrangeParameter", 
		"PlayerHP",
		"PlayerAttackPower",
		"PlayerDefensePower",
		"PlayerMoveSpeed",
		"Data/Parameter/Character/Player/Orange/", 
		"Orange/OrangeModel")
{
	this->operatorSenderId = operatorSenderId;
	isOwnOperator = operatorSenderId == Net::GetIns().GetSenderId();
}


void OrangePlayer::PlayerLoad(void)
{

#pragma region モデル

	MV1SetSpcColorScale(trans.model, GetColorF(0.0f, 0.0f, 0.0f, 1.0f));
	MV1SetDifColorScale(trans.model, GetColorF(0.0f, 0.0f, 0.0f, 1.0f));

	// アニメーション～～～～～～～～～～～～～～～～～～～～～～～～～～～～

	// アニメーションコントローラーを生成する
	CreateAnimationController();

	// 各種アニメーションを登録する
	for (int i = 0; i < (int)ANIME_TYPE::MAX; i++) {
		AddAnimation(i, ANIME_SPEED_TABLE[i], ANIME_PATH_TABLE[i].c_str());
	}

	// ～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～

#pragma endregion

#pragma region プレイヤーが抱える下位クラスを生成する

	// 三弾攻撃の当たり判定オペレーターを生成する
	subObjArray.emplace_back(
		new PlayerTripleAttackCollOperator(
			SKILL_1_TARGET_SERCH_RANGE,
			COLLIDER_TAG::PLAYER_ATTACK,
			SKILL_1_COLL_SIZE_TABLE,
			SKILL_1_COLL_LOCAL_POS,
			SKILL_1_ATTACK_RATE_PERCENT,
			trans.pos, trans.angle,
			characterStats
		)
	);

	// キック用の当たり判定オペレーターを生成する
	subObjArray.emplace_back(
		new PlayerSimpleAttackCollOperator(
			SKILL_2_TARGET_SERCH_RANGE,
			SKILL_2_COLL_TAG,
			SKILL_2_COLL_SIZE_TABLE,
			SKILL_2_COLL_LOCAL_POS,
			trans.pos, trans.angle,
			SKILL_2_ATTACK_RATE_PERCENT,
			characterStats
		)
	);

#pragma endregion


#pragma region 状態設定

	// 移動状態を追加する
	AddState(
		(int)STATE::MOVE,
		new PlayerMoveState(
			// 自分の状態に遷移する関数
			[&]() { state = (int)STATE::MOVE; },
			// 自分の状態かどうかを返す関数
			[&]() { return state == (int)STATE::MOVE; },
			// 定数（加算移動量 / 移動量の最大値 / ダッシュの移動量倍率 / スタミナ量 / 加速減衰量）
			DASH_SPEED_RATE, DASH_STAMINA_MAX, ATTENUATION,
			// 参照（移動量 / 横軸加速度の最大値 / 角度）
			accelSum, ACCEL_MAX, trans.angle, characterStats,
			// アニメーションの再生関数のポインタ（待機 / 歩き / 走り）
			[&]() { AnimePlay((int)ANIME_TYPE::IDLE); },
			[&]() { AnimePlay((int)ANIME_TYPE::WALK); },
			[&]() { AnimePlay((int)ANIME_TYPE::RUN); }
		)
	);

	// 三弾攻撃状態用のコンテキスト構造体を定義して設定～～～～～～～～～～～
	PlayerTripleAttackStateContext stateSkill1Context;

	// キー
	stateSkill1Context.ATTACK_KEY = KEY_TYPE::PLAYER_SKILL_1;
	// クールタイム
	stateSkill1Context.COOL_TIME = SKILL_1_COOL_TIME;
	// 攻撃が次段に繋がる猶予時間
	stateSkill1Context.ATTACK_NEXT_STAGE_CONTINUE_TIME = SKILL_1_ATTACK_NEXT_STAGE_CONTINUE_TIME;
	// 攻撃の判定を発生させる開始時間（アニメーションの再生割合）
	stateSkill1Context.COLL_START_TIME = SKILL_1_COLL_START_TIME;
	// 攻撃の判定を発生させる終了時間（アニメーションの再生割合）
	stateSkill1Context.COLL_END_TIME = SKILL_1_COLL_END_TIME;
	// 攻撃中の移動速度
	stateSkill1Context.ATTACK_MOVE_SPEED = SKILL_1_ATTACK_MOVE_SPEED;

	// 当たり判定のオペレーター
	stateSkill1Context.collOperator = SubObjSerch<PlayerTripleAttackCollOperator>();
	
	// 座標
	stateSkill1Context.pos = &trans.pos;
	// 角度
	stateSkill1Context.angle = &trans.angle;

	// 攻撃1～3段目アニメーションの再生関数のポインタ
	stateSkill1Context.PlayAttackAnimes = {
		[&]() { AnimePlay((int)ANIME_TYPE::PUNCH1,false); },
		[&]() { AnimePlay((int)ANIME_TYPE::PUNCH2,false); },
		[&]() { AnimePlay((int)ANIME_TYPE::PUNCH3,false); }
	};
	// アニメーションの再生割合を取得する関数のポインタ
	stateSkill1Context.GetAnimePlayRatio = std::bind(&OrangePlayer::GetAnimeRatio, this);
	// アニメーションが終了したかのフラグを取得する関数のポインタ
	stateSkill1Context.IsAnimeEnd = std::bind(&OrangePlayer::IsAnimeEnd, this);

	// 攻撃終了後の状態遷移関数のポインタ
	stateSkill1Context.DefaultChangeState = [&]() { state = (int)STATE::MOVE; };

	// ～～～～～～～～～～～三弾攻撃状態用のコンテキスト構造体を定義して設定

	// 三段攻撃状態を追加する
	AddState(
		(int)STATE::SKILL_1,
		new PlayerTripleAttackState(
			// 自分の状態に遷移する関数
			[&]() { state = (int)STATE::SKILL_1; },
			// 自分の状態かどうかを返す関数
			[&]() { return state == (int)STATE::SKILL_1; },
			// コンテキスト構造体
			stateSkill1Context
		)
	);

	// 攻撃状態を追加する (キック)
	AddState(
		(int)STATE::SKILL_2,
		new PlayerSimpleAttackState(
			// 自分の状態に遷移する関数
			[&]() { state = (int)STATE::SKILL_2; },
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
			[&]() { state = (int)STATE::MOVE; }
		)
	);

	AddState(
		(int)STATE::SKILL_3,
		new PlayerDodgeState(
			// 自分の状態に関する関数
			[&]() { state = (int)STATE::SKILL_3; },
			// 自分の状態かどうかを返す関数
			[&]() { return state == (int)STATE::SKILL_3; },
			// 定数（使用するキー / クールタイム / 回避中の移動速度 / 無敵時間の 開始 / 終了時間（アニメーションの再生割合
			KEY_TYPE::PLAYER_SKILL_3, SKILL_3_COOL_TIME, SKILL_3_MOVE_SPEED,
			SKILL_3_INVI_START_TIME, SKILL_3_INVI_END_TIME,
			// 座標 / 角度
			trans.pos,trans.angle,
			// アニメーションの再生関数のポインタ
			[&]() { AnimePlay((int)ANIME_TYPE::DODGE, false); },
			// アニメーションの再生割合を取得する関数のポインタ / アニメーションの終了フラグを取得する関数のポインタ
			[&]() { return GetAnimeRatio(); }, [&]() { return IsAnimeEnd(); },
			// 無敵時間のセット関数
			std::bind(&OrangePlayer::SetInviCounter, this, std::placeholders::_1),
			// 攻撃終了後の状態遷移関数のポインタ (今回は移動状態に遷移するようにする）
			[&]() { state = (int)STATE::MOVE; }
		)
	);

	AddState(
		(int)STATE::DAMAGE,
		new PlayerDamageState(
			// 自分の状態に関する関数
			[&]() { state = (int)STATE::DAMAGE; },
			// 自分の状態かどうかを返す関数
			[&]() { return state == (int)STATE::DAMAGE; },
			// 定数（ダメージを受けた時の無敵時間）
			DAMAGE_INVI_TIME,
			// アニメーションの再生関数のポインタ
			[&]() { AnimePlay((int)ANIME_TYPE::DAMAGE, false); },
			// アニメーションの終了フラグを取得する関数のポインタ
			[&]() { return IsAnimeEnd(); },
			// 無敵時間のセット関数
			std::bind(&OrangePlayer::SetInviCounter, this, std::placeholders::_1),
			// 攻撃終了後の状態遷移関数のポインタ (今回は移動状態に遷移するようにする）
			[&]() { state = (int)STATE::MOVE; }
		)
	);

	// 死亡状態を追加する
	AddState(
		(int)STATE::DEATH,
		new PlayerDeathState(
			[&]() { state = (int)STATE::DEATH; },
			[&]() { return state == (int)STATE::DEATH; },
			trans.pos, trans.angle,
			[&]() { return IsAnimeEnd(); },
			[&]() { AnimePlay((int)ANIME_TYPE::DEATH, false); },
			[&]() {	Camera::GetIns().ChangeModeFixedPoint(trans.pos + Vector3::YZonly(250, -550), Deg2Rad(30)); },
			[&]() { isDeath = true; }
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

	// HPの登録
	ui_ArrayIns.emplace_back(
		new CharacterHpUI(
			GetCharacterStats() , 
			CharacterHpUI::CHARACTER_KINDS::PLAYER,
			"OrangePlayer"
		)
	);

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
			PlayerSkillUI::SKILL_UI_COLOR::RED,
			"SkillSlotTripleAttack"
		)
	);

	// スキル2のUI
	ui_ArrayIns.emplace_back(
		new PlayerSkillUI(
			SKILL2_UI_DRAW_POS,
			dynamic_cast<PlayerSimpleAttackState*>(&GetStateIns((int)STATE::SKILL_2))->GetCoolTimeCounter(),
			SKILL_2_COOL_TIME,
			PlayerSkillUI::SKILL_UI_COLOR::BLUE,
			"SkillSlotSimpleAttack"
		)
	);

	// スキル3のUI
	ui_ArrayIns.emplace_back(
		new PlayerSkillUI(
			SKILL3_UI_DRAW_POS,
			dynamic_cast<PlayerDodgeState*>(&GetStateIns((int)STATE::SKILL_3))->GetCoolTimeCounter(),
			SKILL_3_COOL_TIME, 
			PlayerSkillUI::SKILL_UI_COLOR::GREEN,
			"SkillSlotDodge"
		)
	);

#pragma endregion 
}


void OrangePlayer::ReceptionUpdate(void)
{
	while (MsgDataPlayerTrans* dataPtr = Net::GetIns().GetMsgData<MsgDataPlayerTrans>(operatorSenderId)) {
		// 自分のキャラ（操作対象）の場合
		if (isOwnOperator) {
			// ホストから送られた座標と今の自分の座標の距離を測る
			float diff = (trans.pos, dataPtr->pos).Length();

			// 誤差が小さいなら無視
			if (diff > 0.5f) {
				// 誤差が大きい場合、少しずつホストから送られた座標に寄せる（補間）
				trans.pos = trans.pos * 0.9f + dataPtr->pos * 0.1f;
			}
		}
		// 他人のキャラなら、そのまま同期
		else { trans.pos = dataPtr->pos; }

		// 角度を同期
		trans.angle = dataPtr->angle;
		delete dataPtr;
	}
}

void OrangePlayer::SendUpdate(void)
{
	if (Net::GetIns().IsHost() || isOwnOperator) {
		Net::GetIns().Send(MsgDataPlayerTrans(trans.pos, trans.angle), operatorSenderId);
	}
}

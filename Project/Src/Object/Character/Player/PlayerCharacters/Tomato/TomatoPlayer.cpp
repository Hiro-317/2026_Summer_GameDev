#include "TomatoPlayer.h"

#include "UniqueState/Move/TomatoPlayerMoveState.h"
#include "UniqueState/Tackle/TomatoPlayerTackleState.h"
#include "UniqueState/Damage/TomatoPlayerDamageState.h"
#include "../../CommonPlayerState/OtherPlayerWatch/OtherPlayerWatchState.h"

#include "../../../../UI/PlayerStaminaUI/PlayerStaminaUI.h"
#include "../../../../UI/CharacterHpUI/CharacterHpUI.h"
#include "../../../../UI/PlayerSkillUI/PlayerSkillUI.h"

TomatoPlayer::TomatoPlayer(MSG_SENDER_ID operatorSenderId) :
	PlayerBase(
		operatorSenderId,

		"TomatoParameter",
		"PlayerHP",
		"PlayerAttackPower",
		"PlayerDefensePower",
		"PlayerMoveSpeed",

		"Data/Parameter/Character/Player/Tomato/",
		"Tomato/Tomato")
{
}

void TomatoPlayer::PlayerLoad(void)
{
	// 影を消す（消さなかったら、変な色合いになるので）
	MV1SetSpcColorScale(trans.model, GetColorF(0.0f, 0.0f, 0.0f, 1.0f));
	MV1SetDifColorScale(trans.model, GetColorF(0.0f, 0.0f, 0.0f, 1.0f));

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
			accelSum, ACCEL_MAX, trans.angle, characterStats
		)
	);

	// 観戦モード
	AddState(
		(int)STATE::OTHER_WATCH,
		new OtherPlayerWatchState(
			// 自分の状態に遷移する関数
			[&]() { ChangeState((int)STATE::OTHER_WATCH); },
			// 自分の状態かどうかを返す関数
			[&]() { return state == (int)STATE::OTHER_WATCH; },
			// 他プレイヤーの座標
			otherPlayerTrans,
			// ボスの座標
			bossPos
		)
	);

	AddState(
		(int)STATE::SKILL_1,
		new TomatoPlayerTackleState(
			// 自分の状態に遷移する関数
			[&]() { ChangeState((int)STATE::SKILL_1); },
			// 自分の状態かどうかを返す関数
			[&]() { return state == (int)STATE::SKILL_1; },
			// クールタイム
			SKILL1_COOL_TIME,
			// 移動速度 / 回転速度
			MOVE_SPEED, ROTATION_POW,
			// 座標 / 角度
			trans.pos, trans.angle,
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
			trans.angle,
			// 無敵時間のセット関数
			[&]() { SetInviCounter(60); },
			// 攻撃終了後の状態遷移関数のポインタ (今回は移動状態に遷移するようにする）
			[&]() { ChangeState((int)STATE::MOVE); }
		)
	);


	// 遷移条件の登録（before = 遷移元)(after = 遷移後）
	auto AddChangeStateCondition = [&](STATE before, STATE after)->void {
		GetStateIns((int)before).AddOtherStateCondition([this, after](void) { GetStateIns((int)after).OwnStateConditionUpdate(); });
		};

	// 移動状態 -> スキル1 の遷移を登録
	AddChangeStateCondition(STATE::MOVE, STATE::SKILL_1);

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
				dynamic_cast<TomatoPlayerTackleState*>(&GetStateIns((int)STATE::SKILL_1))->GetCoolTimeCounter(),
				// クールタイムの最大値
				SKILL1_COOL_TIME,
				// UIの色指定
				PlayerSkillUI::SKILL_UI_COLOR::RED,
				// 描画する画像
				"SkillSlotTripleAttack"
			)
		);
	}


}

void TomatoPlayer::ReceptionUpdate(void)
{
}

void TomatoPlayer::SendUpdate(void)
{
}

#include "PeachPlayer.h"

#include "../../../../UI/PlayerStaminaUI/PlayerStaminaUI.h"
#include "../../../../UI/CharacterHpUI/CharacterHpUI.h"
#include "../../../../UI/PlayerSkillUI/PlayerSkillUI.h"
#include "../../../../UI/HitUI/HitUI.h"

PeachPlayer::PeachPlayer(MSG_SENDER_ID operatorSenderId) :
	PlayerBase(
		operatorSenderId,

		"PeachParameter",
		"PlayerHP",
		"PlayerAttackPower",
		"PlayerDefensePower",
		"PlayerMoveSpeed",

		"Data/Parameter/Character/Player/Tomato/",
		"Tomato/Tomato"
	)
{
}

void PeachPlayer::PlayerLoad(void)
{
	// 影を消す（消さなかったら、変な色になるので）
	MV1SetSpcColorScale(trans.model, GetColorF(0.0f, 0.0f, 0.0f, 1.0f));
	MV1SetDifColorScale(trans.model, GetColorF(0.0f, 0.0f, 0.0f, 1.0f));

#pragma region 下位オブジェクトの生成


#pragma endregion 

#pragma region 状態設定

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
	}

	ui_ArrayIns.emplace_back(new HitUI(trans.pos));
}

void PeachPlayer::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other)
{
	if (!Net::GetIns().IsHost()) { return; }
	if (GetInviCounter() > 0) { return; }
	if (state == (int)STATE::DEATH) { return; }

	switch (other.GetTag()) {
	case COLLIDER_TAG::BOSS_ATTACK: {		// ボスの攻撃
		// ダメージ状態に遷移
		ChangeState((int)STATE::DAMAGE);
		// ボスの攻撃力とプレイヤーの防御力で、最終的なダメージ値を計算
		const short damage = CalculateDamage(other.GetSkillStats().Power(), characterStats.defensePower.Value());
		// プレイヤーが受けるダメージ値を、クライアント側に送信
		Net::GetIns().Send(MsgDataPlayerDamage(damage), operatorSenderId);
		// ダメージ値分HPを減らす
		characterStats.hp -= damage;
		break;
	}
	}
}

void PeachPlayer::ReceptionUpdate(void)
{
	PlayerBase::ReceptionUpdate();
}

void PeachPlayer::SendUpdate(void)
{
	PlayerBase::SendUpdate();
}

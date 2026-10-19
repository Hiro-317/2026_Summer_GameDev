#include "PlayerSkillUI.h"

#include "../../../../pch.h"

PlayerSkillUI::PlayerSkillUI(
	const Vector2I& pos,
	int COOL_TIME,
	const float& coolTimeCounter,
	SKILL_UI_COLOR color,
	const std::string& skillImageName,
	bool isChargeSkill
) :
	pos(pos),

	COOL_TIME(COOL_TIME),
	coolTimeCounter(coolTimeCounter),

	isChargeSkill(isChargeSkill)
{
	// 画像の読み込み（枠）
	frameImage = LoadGraph((IMAGE_FILE_PATH + "SkillSlotFrame" + ".png").c_str());
	// 画像の読み込み（チャージ中背景）
	chargingBackImage = LoadGraph((IMAGE_FILE_PATH + CHARGING_IMAGE_NAME_TABLE.at(color) + ".png").c_str());
	// 画像の読み込み（チャージ完了背景）
	chargeCompleteBackImage = LoadGraph((IMAGE_FILE_PATH + CHARGE_IMAGE_NAME_TABLE.at(color) + ".png").c_str());
	// 画像の読み込み（スキル）
	skillImage = LoadGraph((IMAGE_FILE_PATH + skillImageName + ".png").c_str());
	// 画像の読み込み（チャージ可能）（特殊）
	reloadableImage = isChargeSkill ? LoadGraph((IMAGE_FILE_PATH + "ChargeOK" + ".png").c_str()) : -1;
}

void PlayerSkillUI::SubInit(void)
{
	// UI描画に設定する
	SetDrawType(ACTOR_DRAW_TYPE::Ui);
}

void PlayerSkillUI::SubDraw(void)
{
	// 描画（枠）
	DrawRotaGraph(pos.x, pos.y, 1, 0, frameImage, true);

	// チャージ割合を計算
	const float chargeRatio = coolTimeCounter / (float)COOL_TIME;

	// クールタイム中の場合
	if (chargeRatio > 0.0f) {

		// チャージ割合による画像の縦方向のズレ
		const int offset = (int)((float)IMAGE_SIZE.y * (1.0f - chargeRatio));

		// 描画（チャージ中背景）
		DrawRectGraph(
			pos.x - (IMAGE_SIZE.x / 2),
			pos.y + offset,

			0,
			offset,

			IMAGE_SIZE.x,
			IMAGE_SIZE.y - offset,

			chargingBackImage,

			true
		);
		
	}
	// クールタイムが終了している場合
	else {
		// 描画（背景）
		DrawRotaGraph(pos.x, pos.y, 1, 0, chargingBackImage, true);
		DrawRotaGraph(pos.x, pos.y, 1, 0, chargeCompleteBackImage, true);
		DrawRotaGraph(pos.x, pos.y - (IMAGE_SIZE.y / 2) - (int)(512.0f * 0.1f), 0.1f, 0, reloadableImage, true);
	}

	// 描画（スキル）
	DrawRotaGraph(pos.x, pos.y, 1, 0, skillImage, true);
}

void PlayerSkillUI::SubRelease(void)
{
	// 画像の解放（チャージ可能）（特殊）
	if (reloadableImage != -1) {
		DeleteGraph(reloadableImage);
		reloadableImage = -1;
	}
	// 画像の解放（スキル）
	if (skillImage != -1) {
		DeleteGraph(skillImage);
		skillImage = -1;
	}
	// 画像の解放（チャージ完了背景）
	if (chargeCompleteBackImage != -1) {
		DeleteGraph(chargeCompleteBackImage);
		chargeCompleteBackImage = -1;
	}
	// 画像の解放（チャージ中背景）
	if (chargingBackImage != -1) {
		DeleteGraph(chargingBackImage);
		chargingBackImage = -1;
	}
	// 画像の解放（枠）
	if (frameImage != -1) {
		DeleteGraph(frameImage);
		frameImage = -1;
	}
}
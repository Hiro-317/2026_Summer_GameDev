#include "CharacterHpUI.h"

CharacterHpUI::CharacterHpUI(const CharacterStats& stats, const CHARACTER_KINDS characterKinds) :
	playerhp(stats.hp),
	PLAYER_HP_MAX(stats.hpMax.Value()),
	CHARA_KINDS(characterKinds), // ここで初期化
	hpRatio(0.0f),
	hpBarOffset(0.0f),
	damageBarOffset(0.0f)
{

}

void CharacterHpUI::Load(void)
{
	// 画像のロード
	UILoadImage(HP_FRAME_IMAGE_NAME[(int)CHARA_KINDS], (int)IMAGE_KINDS::FRAME, HP_IMAGE_FILE_PATH_TYPE[(int)CHARA_KINDS]);
	UILoadImage(HP_IMAGE_NAME[(int)CHARA_KINDS], (int)IMAGE_KINDS::HP_GAUGE, HP_IMAGE_FILE_PATH_TYPE[(int)CHARA_KINDS]);
	UILoadImage(HP_LOST_IMAGE_NAME[(int)CHARA_KINDS], (int)IMAGE_KINDS::DAMAGE_GAUGE, HP_IMAGE_FILE_PATH_TYPE[(int)CHARA_KINDS]);
}

void CharacterHpUI::SubUpdate()
{
	// HPの割合によるHPバーの増減のための計算
	hpRatio = (float)playerhp / (float)PLAYER_HP_MAX;
	hpBarOffset = HP_IMAGE_SIZE[(int)CHARA_KINDS].x * (1.0f - hpRatio);

	if (damageBarOffset < hpBarOffset) {
		// ダメージを受けたので赤いゲージを後から減少
		damageBarOffset += DAMAGE_GAUGE_DEC;
		// HPを上回って減らないようにする
		if (damageBarOffset > hpBarOffset) {
			damageBarOffset = hpBarOffset;
		}
	}
}

void CharacterHpUI::SubDraw()
{
	Vector2I uiPos = DEFAULT_UI_POS[(int)CHARA_KINDS];

	// HPのフレーム画像描画
	DrawGraph(uiPos.x, uiPos.y, uiImages.at((int)IMAGE_KINDS::FRAME), true);

	// ダメージを受けたときの赤いゲージ
	DrawRectGraph(
		uiPos.x + HP_GAUGE_OFFSET[(int)CHARA_KINDS],
		uiPos.y,
		0, 0,
		HP_IMAGE_SIZE[(int)CHARA_KINDS].x - damageBarOffset,
		HP_IMAGE_SIZE[(int)CHARA_KINDS].y,
		uiImages.at((int)IMAGE_KINDS::DAMAGE_GAUGE),
		true
	);

	// HPバーの描画
	DrawRectGraph(
		uiPos.x + HP_GAUGE_OFFSET[(int)CHARA_KINDS],
		uiPos.y,
		0, 0,
		HP_IMAGE_SIZE[(int)CHARA_KINDS].x - hpBarOffset, 
		HP_IMAGE_SIZE[(int)CHARA_KINDS].y,
		uiImages.at((int)IMAGE_KINDS::HP_GAUGE),
		true
	);
}

void CharacterHpUI::SubRelease()
{
}
#include "CharacterHpUI.h"

#include "../../../Manager/Font/FontManager.h"

CharacterHpUI::CharacterHpUI(
	const CharacterStats& stats,
	const CHARACTER_KINDS characterKinds, 
	const int PLAYER_NUMBER, 
	const std::string CHARA_NAME
) :
	playerhp(stats.hp),
	PLAYER_HP_MAX(stats.hpMax.Value()),
	CHARA_KINDS(characterKinds), // ここで初期化
	hpRatio(0.0f),
	hpBarOffset(0.0f),
	damageBarOffset(0.0f),
	PLAYER_NUMBER(PLAYER_NUMBER),
	CHARA_NAME(CHARA_NAME)
{

}

CharacterHpUI::CharacterHpUI(
	const CharacterStats& stats,

	const char* HP_FRAME_IMAGE_NAME,
	const char* HP_IMAGE_NAME,
	const char* HP_LOST_IMAGE_NAME,

	const Vector2I& HP_IMAGE_SIZE,
	short HP_GAUGE_OFFSET,
	const Vector2I& HP_UI_POS,

	const std::string CHARA_NAME
):
	playerhp(stats.hp),
	HP_MAX(stats.hpMax.Value()),


	CHARA_NAME(CHARA_NAME),

	hpRatio(0.0f),
	hpBarOffset(0.0f),
	damageBarOffset(0.0f)
{
}

void CharacterHpUI::Load(void)
{
	// 画像のロード
	UILoadImage(HP_FRAME_IMAGE_NAME, (int)IMAGE_KINDS::FRAME, HP_IMAGE_FILE_PATH_TYPE[(int)CHARA_KINDS]);
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

	// 
	int color = ((bool)((int)CHARA_KINDS)) ? 0xff0000 : 0xffffff;
	DrawFormatStringToHandle(uiPos.x + 20, uiPos.y + ((HP_IMAGE_SIZE[(int)CHARA_KINDS].y / 2) - 10), color, Font::GetIns().GetFont(FontKinds::DEFAULT_20), (CHARA_NAME).c_str());
}

void CharacterHpUI::SubRelease()
{
}
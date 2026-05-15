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
	UILoadImage("PlayerHpFrame", (int)IMAGE_KINDS::FRAME,			FILE_PATH_TYPE::HP);
	UILoadImage("PlayerHp",		 (int)IMAGE_KINDS::HP_GAUGE,		FILE_PATH_TYPE::HP);
	UILoadImage("PlayerHpLost",  (int)IMAGE_KINDS::DAMAGE_GAUGE,	FILE_PATH_TYPE::HP);
}

void CharacterHpUI::SubUpdate()
{
	// HPの割合によるHPバーの増減のための計算
	hpRatio = (float)playerhp / (float)PLAYER_HP_MAX;
	hpBarOffset = HP_IMAGE_SIZE.x * (1.0f - hpRatio);

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
	// HPのフレーム画像描画
	DrawGraph(HP_UI_POS.x, HP_UI_POS.y, uiImages.at((int)IMAGE_KINDS::FRAME), true);

	// ダメージを受けたときの赤いゲージ
	DrawRectGraph(
		HP_UI_POS.x + HP_GAUGE_OFFSET[(int)CHARA_KINDS],
		HP_UI_POS.y,
		0, 0,
		HP_IMAGE_SIZE.x - damageBarOffset,
		HP_IMAGE_SIZE.y,
		uiImages.at((int)IMAGE_KINDS::DAMAGE_GAUGE),
		true
	);

	// HPバーの描画
	DrawRectGraph(
		HP_UI_POS.x + HP_GAUGE_OFFSET[(int)CHARA_KINDS],
		HP_UI_POS.y,
		0, 0,
		HP_IMAGE_SIZE.x - hpBarOffset, HP_IMAGE_SIZE.y,
		uiImages.at((int)IMAGE_KINDS::HP_GAUGE),
		true
	);
}

void CharacterHpUI::SubRelease()
{
}